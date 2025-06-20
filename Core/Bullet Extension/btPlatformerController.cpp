#include "btPlatformerController.h"

#include <stdio.h>
#include "LinearMath/btIDebugDraw.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"

const btScalar terminalVelocity = 55.0f;
const btScalar maxPenetrationDepth = 0.2f;

#pragma region Static Helper Methods
static btVector3 getNormalizedVector(const btVector3& v)
{
	btVector3 n(0, 0, 0);
	if (v.length() > SIMD_EPSILON)
		n = v.normalized();
	return n;
};

//Returns the reflection direction of a ray going 'direction' hitting a surface with normal 'normal'.
//From: http://www-cs-students.stanford.edu/~adityagp/final/node3.html
static btVector3 computeReflectionDirection(const btVector3& direction, const btVector3& normal)
{
	return direction - (btScalar(2.0) * direction.dot(normal)) * normal;
};

//Returns the portion of 'direction' that is parallel to 'normal'.
static btVector3 parallelComponent(const btVector3& direction, const btVector3& normal)
{
	btScalar magnitude = direction.dot(normal);
	return normal * magnitude;
};

//Returns the portion of 'direction' that is perpindicular to 'normal'
static btVector3 perpendicularComponent(const btVector3& direction, const btVector3& normal)
{
	return direction - parallelComponent(direction, normal);
};

static btQuaternion getRotation(btVector3& v0, btVector3& v1)
{
	if (v0.length2() == 0.0f || v1.length2() == 0.0f)
	{
		btQuaternion q;
		return q;
	}
	return shortestArcQuatNormalize2(v0, v1);
};

static bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1)
{
	bool collides = (body0->getBroadphaseHandle()->m_collisionFilterGroup & body1->getBroadphaseHandle()->m_collisionFilterMask) != 0;
	return collides && (body1->getBroadphaseHandle()->m_collisionFilterGroup & body0->getBroadphaseHandle()->m_collisionFilterMask);
};
#pragma endregion

#pragma region Callback Class
class btKinematicClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	btKinematicClosestNotMeConvexResultCallback(btCollisionObject* me, const btVector3& up, btScalar minSlopeDot)
		: btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)), m_me(me), m_up(up), m_minSlopeDot(minSlopeDot)
	{
		m_collisionFilterGroup = me->getBroadphaseHandle()->m_collisionFilterGroup;
		m_collisionFilterMask = me->getBroadphaseHandle()->m_collisionFilterMask;
	};

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == m_me)
			return btScalar(1.0);

		if (!convexResult.m_hitCollisionObject->hasContactResponse())
			return btScalar(1.0);

		btVector3 hitNormalWorld;
		if (normalInWorldSpace)
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		}
		else
		{
			///need to transform normal into worldspace
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up.dot(hitNormalWorld);
		if (dotUp < m_minSlopeDot)
		{
			return btScalar(1.0);
		}

		return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	};

protected:
	btCollisionObject* m_me;
	const btVector3 m_up;
	btScalar m_minSlopeDot;
};
#pragma endregion

#pragma region Constructor and Destructor
btPlatformerController::btPlatformerController(btPairCachingGhostObject* ghostObject, btConvexShape* convexShape, btScalar stepHeight, const btVector3& up)
{
	m_ghostObject = ghostObject;
	m_convexShape = convexShape;

	m_jumpSpeed = 5.0;
	m_coyoteTime = 0.1f;

	m_gravity = 9.8f * 3.0f;  // 3G acceleration.

	m_walkDirection.setValue(0.0, 0.0, 0.0);
	m_normalizedDirection.setValue(0.0, 0.0, 0.0);

	m_up.setValue(0.0f, 0.0f, 1.0f);
	m_jumpAxis.setValue(0.0f, 0.0f, 1.0f);

	m_currentStepOffset = 0.0;
	m_verticalVelocity = 0.0;
	m_verticalOffset = 0.0;

	m_notGroundedTime = 1.0;
	m_wasJumping = false;

	m_useWalkDirection = true;
	m_velocityTimeInterval = 0.0;

	setUp(up);
	setStepHeight(stepHeight);
	setMaxSlope(btRadians(45.0));
};

btPlatformerController::~btPlatformerController()
{
};
#pragma endregion

#pragma region btActionInterface Implementation
void btPlatformerController::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime)
{
	preStep(collisionWorld);
	playerStep(collisionWorld, deltaTime);
};
#pragma endregion

#pragma region btCharacterControllerInterface Implementation
void btPlatformerController::setWalkDirection(const btVector3& walkDirection)
{
	m_useWalkDirection = true;
	m_walkDirection = walkDirection;
	m_normalizedDirection = getNormalizedVector(m_walkDirection);
};

void btPlatformerController::setVelocityForTimeInterval(const btVector3& velocity, btScalar timeInterval)
{
	m_useWalkDirection = false;
	m_walkDirection = velocity;
	m_normalizedDirection = getNormalizedVector(m_walkDirection);
	m_velocityTimeInterval += timeInterval;
};

void btPlatformerController::reset(btCollisionWorld* collisionWorld)
{
	m_verticalVelocity = 0.0;
	m_verticalOffset = 0.0;
	m_notGroundedTime = 1.0;
	m_wasJumping = false;
	m_walkDirection.setValue(0, 0, 0);
	m_velocityTimeInterval = 0.0;

	//clear pair cache
	btHashedOverlappingPairCache* cache = m_ghostObject->getOverlappingPairCache();
	while (cache->getOverlappingPairArray().size() > 0)
	{
		cache->removeOverlappingPair(cache->getOverlappingPairArray()[0].m_pProxy0, cache->getOverlappingPairArray()[0].m_pProxy1, collisionWorld->getDispatcher());
	}
};

void btPlatformerController::warp(const btVector3& origin)
{
	btTransform xform;
	xform.setIdentity();
	xform.setOrigin(origin);
	m_ghostObject->setWorldTransform(xform);
};

void btPlatformerController::preStep(btCollisionWorld* collisionWorld)
{
	m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();
	m_targetPosition = m_currentPosition;
};

void btPlatformerController::playerStep(btCollisionWorld* collisionWorld, btScalar dt)
{
	//Early out if no move required.
	if (!m_useWalkDirection && (m_velocityTimeInterval <= 0.0 || m_walkDirection.fuzzyZero()))
		return;

	if (onGround())
		m_notGroundedTime = 0.0;
	else
		m_notGroundedTime += dt;

	//Update fall velocity.
	m_verticalVelocity -= m_gravity * dt;
	if (m_verticalVelocity < -terminalVelocity)
		m_verticalVelocity = -terminalVelocity;
	m_verticalOffset = m_verticalVelocity * dt;

	btTransform xform;
	xform = m_ghostObject->getWorldTransform();

	stepUp(collisionWorld);

	if (m_useWalkDirection)
	{
		stepForwardAndStrafe(collisionWorld, m_walkDirection);
	}
	else
	{
		btScalar availableMoveTime = (dt < m_velocityTimeInterval) ? dt : m_velocityTimeInterval;
		m_velocityTimeInterval -= dt;
		btVector3 moveDistance = m_walkDirection * availableMoveTime;
		stepForwardAndStrafe(collisionWorld, moveDistance);
	}

	stepDown(collisionWorld, dt);

	xform.setOrigin(m_currentPosition);
	m_ghostObject->setWorldTransform(xform);

	resolvePenetration(collisionWorld);
};

bool btPlatformerController::canJump() const
{
	return m_notGroundedTime < m_coyoteTime;
};

void btPlatformerController::jump(const btVector3& v)
{
	m_jumpSpeed = v.length2() == 0 ? m_jumpSpeed : v.length();
	m_verticalVelocity = m_jumpSpeed;
	m_wasJumping = true;
	m_notGroundedTime = 1.0f;

	m_jumpAxis = v.length2() == 0 ? m_up : v.normalized();
};

bool btPlatformerController::onGround() const
{
	return (fabs(m_verticalVelocity) < SIMD_EPSILON) && (fabs(m_verticalOffset) < SIMD_EPSILON);
};
#pragma endregion

#pragma region Getters and Setters
void btPlatformerController::setUp(const btVector3& up)
{
	if (m_up == up)
		return;

	btVector3 u = m_up;

	if (up.length2() > 0)
		m_up = up.normalized();
	else
		m_up = btVector3(0.0, 0.0, 0.0);

	if (!m_ghostObject) return;
	btQuaternion rot = getRotation(m_up, u);

	//set orientation with new up
	btTransform xform;
	xform = m_ghostObject->getWorldTransform();
	btQuaternion orn = rot.inverse() * xform.getRotation();
	xform.setRotation(orn);
	m_ghostObject->setWorldTransform(xform);
};

void btPlatformerController::setLinearVelocity(const btVector3& velocity)
{
	m_walkDirection = velocity;

	// HACK: if we are moving in the direction of the up, treat it as a jump :(
	if (m_walkDirection.length2() > 0)
	{
		btVector3 w = velocity.normalized();
		btScalar c = w.dot(m_up);
		if (c != 0)
		{
			//there is a component in walkdirection for vertical velocity
			btVector3 upComponent = m_up * (btSin(SIMD_HALF_PI - btAcos(c)) * m_walkDirection.length());
			m_walkDirection -= upComponent;
			m_verticalVelocity = (c < 0.0f ? -1 : 1) * upComponent.length();

			if (c > 0.0f)
			{
				m_wasJumping = true;
			}
		}
	}
	else
		m_verticalVelocity = 0.0f;
};

btVector3 btPlatformerController::getLinearVelocity() const
{
	return m_walkDirection + (m_verticalVelocity * m_up);
};

void btPlatformerController::setMaxSlope(btScalar slopeRadians)
{
	m_maxSlopeRadians = slopeRadians;
	m_maxSlopeCosine = btCos(slopeRadians);
};

btScalar btPlatformerController::getMaxSlope() const
{
	return m_maxSlopeRadians;
};

void btPlatformerController::setGravity(const btVector3& gravity)
{
	if (gravity.length2() > 0)
		setUp(-gravity);
	m_gravity = gravity.length();
};

btVector3 btPlatformerController::getGravity() const
{
	return -m_gravity * m_up;
};
#pragma endregion

#pragma region Internal Methods
void btPlatformerController::resolvePenetration(btCollisionWorld* collisionWorld, int numberOfLoops)
{
	m_touchingContact = false;
	while (recoverFromPenetration(collisionWorld) && numberOfLoops > 0)
	{
		numberOfLoops--;
		m_touchingContact = true;
	}
};

bool btPlatformerController::recoverFromPenetration(btCollisionWorld* collisionWorld)
{
	// Here we must refresh the overlapping paircache as the penetrating movement itself or the
	// previous recovery iteration might have used setWorldTransform and pushed us into an object
	// that is not in the previous cache contents from the last timestep, as will happen if we
	// are pushed into a new AABB overlap. Unhandled this means the next convex sweep gets stuck.
	//
	// Do this by calling the broadphase's setAabb with the moved AABB, this will update the broadphase
	// paircache and the ghostobject's internal paircache at the same time.    /BW

	btVector3 minAabb, maxAabb;
	m_convexShape->getAabb(m_ghostObject->getWorldTransform(), minAabb, maxAabb);
	collisionWorld->getBroadphase()->setAabb(m_ghostObject->getBroadphaseHandle(),
		minAabb,
		maxAabb,
		collisionWorld->getDispatcher());

	bool penetration = false;

	collisionWorld->getDispatcher()->dispatchAllCollisionPairs(m_ghostObject->getOverlappingPairCache(), collisionWorld->getDispatchInfo(), collisionWorld->getDispatcher());

	m_currentPosition = m_ghostObject->getWorldTransform().getOrigin();

	for (int i = 0; i < m_ghostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
	{
		m_manifoldArray.resize(0);

		btBroadphasePair* collisionPair = &m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

		btCollisionObject* obj0 = static_cast<btCollisionObject*>(collisionPair->m_pProxy0->m_clientObject);
		btCollisionObject* obj1 = static_cast<btCollisionObject*>(collisionPair->m_pProxy1->m_clientObject);

		if (!obj0->hasContactResponse() || !obj1->hasContactResponse())
			continue;

		if (!needsCollision(obj0, obj1))
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);

		for (int j = 0; j < m_manifoldArray.size(); j++)
		{
			btPersistentManifold* manifold = m_manifoldArray[j];
			btScalar directionSign = manifold->getBody0() == m_ghostObject ? btScalar(-1.0) : btScalar(1.0);
			for (int p = 0; p < manifold->getNumContacts(); p++)
			{
				const btManifoldPoint& pt = manifold->getContactPoint(p);

				btScalar dist = pt.getDistance();

				if (dist < -maxPenetrationDepth)
				{
					m_currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);
					penetration = true;
				}
			}
		}
	}
	btTransform newTrans = m_ghostObject->getWorldTransform();
	newTrans.setOrigin(m_currentPosition);
	m_ghostObject->setWorldTransform(newTrans);
	return penetration;
};

void btPlatformerController::stepUp(btCollisionWorld* world)
{
	btScalar stepHeight = m_verticalVelocity < 0 ? m_stepHeight : 0.0f;
	btScalar verticalOffset = m_verticalOffset > 0 ? m_verticalOffset : 0.0f;

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	/* FIXME: Handle penetration properly */
	start.setOrigin(m_currentPosition);
	m_targetPosition = m_currentPosition + (m_up * stepHeight) + (m_jumpAxis * verticalOffset);
	m_currentPosition = m_targetPosition;
	end.setOrigin(m_targetPosition);

	btKinematicClosestNotMeConvexResultCallback callback(m_ghostObject, -m_up, m_maxSlopeCosine);
	m_ghostObject->convexSweepTest(m_convexShape, start, end, callback, world->getDispatchInfo().m_allowedCcdPenetration);

	if (callback.hasHit() && m_ghostObject->hasContactResponse() && needsCollision(m_ghostObject, callback.m_hitCollisionObject))
	{
		// Only modify the position if the hit was a slope and not a wall or ceiling.
		if (callback.m_hitNormalWorld.dot(m_up) > 0.0)
		{
			// we moved up only a fraction of the step height
			m_currentStepOffset = stepHeight * callback.m_closestHitFraction;
			m_currentPosition.setInterpolate3(m_currentPosition, m_targetPosition, callback.m_closestHitFraction);
		}

		btTransform& xform = m_ghostObject->getWorldTransform();
		xform.setOrigin(m_currentPosition);
		m_ghostObject->setWorldTransform(xform);

		// fix penetration if we hit a ceiling for example
		resolvePenetration(world);
		m_targetPosition = m_ghostObject->getWorldTransform().getOrigin();
		m_currentPosition = m_targetPosition;

		if (m_verticalOffset > 0)
		{
			m_verticalOffset = 0.0;
			m_verticalVelocity = 0.0;
			m_currentStepOffset = m_stepHeight;
		}
	}
	else
	{
		m_currentStepOffset = stepHeight;
		m_currentPosition = m_targetPosition;
	}
};

void btPlatformerController::stepForwardAndStrafe(btCollisionWorld* collisionWorld, const btVector3& walkMove)
{
	if (walkMove.length2() <= 0)
		return;

	btTransform start, end;
	m_targetPosition = m_currentPosition + walkMove;
	start.setIdentity();
	end.setIdentity();

	btScalar fraction = 1.0;
	btScalar distance2 = (m_currentPosition - m_targetPosition).length2();

	for (int maxIter = 10; maxIter > 0 && fraction > btScalar(0.01); maxIter--)
	{
		start.setOrigin(m_currentPosition);
		end.setOrigin(m_targetPosition);
		btVector3 sweepDirNegative(m_currentPosition - m_targetPosition);

		btKinematicClosestNotMeConvexResultCallback callback(m_ghostObject, sweepDirNegative, btScalar(0.0));
		m_ghostObject->convexSweepTest(m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		fraction -= callback.m_closestHitFraction;

		if (callback.hasHit() && m_ghostObject->hasContactResponse() && needsCollision(m_ghostObject, callback.m_hitCollisionObject))
		{
			updateTargetPositionBasedOnCollision(callback.m_hitNormalWorld);
			btVector3 currentDir = m_targetPosition - m_currentPosition;
			distance2 = currentDir.length2();
			if (distance2 > SIMD_EPSILON)
			{
				currentDir.normalize();
				//See Quake2: "If velocity is against original velocity, stop ead to avoid tiny oscilations in sloping corners."
				if (currentDir.dot(m_normalizedDirection) <= btScalar(0.0))
					break;
			}
			else
				break;
		}
		else
		{
			m_currentPosition = m_targetPosition;
		}

		if ((m_currentPosition - m_targetPosition).fuzzyZero())
			break;
	}
};

void btPlatformerController::updateTargetPositionBasedOnCollision(const btVector3& hitNormal)
{
	btVector3 movementDirection = m_targetPosition - m_currentPosition;
	btScalar movementLength = movementDirection.length();
	if (movementLength > SIMD_EPSILON)
	{
		movementDirection.normalize();

		btVector3 reflectDir = computeReflectionDirection(movementDirection, hitNormal);
		reflectDir.normalize();

		btVector3 parallelDir, perpendicularDir;
		parallelDir = parallelComponent(reflectDir, hitNormal);
		perpendicularDir = perpendicularComponent(reflectDir, hitNormal);

		m_targetPosition = m_currentPosition;

		btVector3 perpComponent = perpendicularDir * btScalar(movementLength);
		m_targetPosition += perpComponent;
	}
};

void btPlatformerController::stepDown(btCollisionWorld* collisionWorld, btScalar dt)
{
	if (m_verticalVelocity > 0.0)
		return;

	btVector3 orig_position = m_targetPosition;

	btScalar downVelocity = (m_verticalVelocity < 0.f ? -m_verticalVelocity : 0.f) * dt;

	//NOTE: TERMINAL VELOCITY. MAYBE NOT NEEDED?
	//if (downVelocity > 0.0 && downVelocity > m_fallSpeed && (m_wasOnGround || !m_wasJumping))
	//	downVelocity = m_fallSpeed;

	btVector3 step_drop = m_up * (m_currentStepOffset + downVelocity);
	m_targetPosition -= step_drop;

	btKinematicClosestNotMeConvexResultCallback callback(m_ghostObject, m_up, m_maxSlopeCosine);
	btKinematicClosestNotMeConvexResultCallback callback2(m_ghostObject, m_up, m_maxSlopeCosine);

	bool smallFall = false;
	for (int runCount = 0; runCount < 2; runCount++)
	{
		btTransform start, end, end_double;
		start.setIdentity();
		end.setIdentity();
		end_double.setIdentity();
		start.setOrigin(m_currentPosition);
		end.setOrigin(m_targetPosition);
		end_double.setOrigin(m_targetPosition - step_drop);//set double test for 2x the step drop, to check for a large drop vs small drop

		m_ghostObject->convexSweepTest(m_convexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

		if (!callback.hasHit() && m_ghostObject->hasContactResponse())
		{
			//test a double fall height, to see if the character should interpolate its fall (full) or not (partial)
			m_ghostObject->convexSweepTest(m_convexShape, start, end_double, callback2, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		}

		btScalar downVelocity2 = (m_verticalVelocity < 0.f ? -m_verticalVelocity : 0.f) * dt;
		bool has_hit = callback2.hasHit() && m_ghostObject->hasContactResponse() && needsCollision(m_ghostObject, callback2.m_hitCollisionObject);

		btScalar stepHeight = m_verticalVelocity < 0.0 ? m_stepHeight : 0.0;

		//Redo the velocity calculation when falling a small amount, for fast stairs motion.
		if (runCount == 0 && downVelocity2 > 0.0 && downVelocity2 < stepHeight && has_hit == true && (m_notGroundedTime == 0 || !m_wasJumping))
		{
			m_targetPosition = orig_position;
			downVelocity = stepHeight;
			step_drop = m_up * (m_currentStepOffset + downVelocity);
			m_targetPosition -= step_drop;
			smallFall = true;
		}
		//For larger falls, use the smoother/slower interpolated movement by not touching the target position.
		else
			break;
	}

	//We dropped part way to the floor.
	if ((m_ghostObject->hasContactResponse() && (callback.hasHit() && needsCollision(m_ghostObject, callback.m_hitCollisionObject))) || smallFall == true)
	{
		btScalar fraction = (m_currentPosition.getY() - callback.m_hitPointWorld.getY()) / 2;

		m_currentPosition.setInterpolate3(m_currentPosition, m_targetPosition, callback.m_closestHitFraction);

		m_verticalVelocity = 0.0;
		m_verticalOffset = 0.0;
		m_wasJumping = false;
	}
	//We dropped the full height.
	else
		m_currentPosition = m_targetPosition;
};
#pragma endregion