#pragma once

#include <LinearMath/btVector3.h>

#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>

class btCollisionShape;
class btConvexShape;
class btRigidBody;
class btCollisionWorld;
class btCollisionDispatcher;
class btPairCachingGhostObject;

ATTRIBUTE_ALIGNED16(class) btPlatformerController : public btCharacterControllerInterface
{
protected:

	btPairCachingGhostObject * m_ghostObject;
	btConvexShape* m_convexShape;  //is also in m_ghostObject, but it needs to be convex, so we store it here to avoid upcast

	btScalar m_stepHeight;
	btScalar m_jumpSpeed;
	btScalar m_coyoteTime;

	btScalar m_maxSlopeRadians;  // Slope angle that is set (used for returning the exact value)
	btScalar m_maxSlopeCosine;   // Cosine equivalent of m_maxSlopeRadians (calculated once when set, for optimization)
	btScalar m_gravity;

	///this is the desired walk direction, set by the user
	btVector3 m_walkDirection;
	btVector3 m_normalizedDirection;

	//some internal variables
	btVector3 m_currentPosition;
	btScalar m_currentStepOffset;
	btVector3 m_targetPosition;
	btScalar m_verticalVelocity;
	btScalar m_verticalOffset;

	///keep track of the contact manifolds
	btManifoldArray m_manifoldArray;
	bool m_touchingContact;//NOTE: DOES NOTHING. POTENTIALLY USEFUL FOR PUSHING OBJECTS?

	double m_notGroundedTime;
	bool m_wasJumping;

	bool m_useWalkDirection;
	btVector3 m_up;
	btScalar m_velocityTimeInterval;
	btVector3 m_jumpAxis;

	void resolvePenetration(btCollisionWorld* collisionWorld, int numberOfLoops = 4);
	bool recoverFromPenetration(btCollisionWorld* collisionWorld);
	void stepUp(btCollisionWorld* collisionWorld);
	void updateTargetPositionBasedOnCollision(const btVector3& hit_normal);
	void stepForwardAndStrafe(btCollisionWorld* collisionWorld, const btVector3& walkMove);
	void stepDown(btCollisionWorld* collisionWorld, btScalar dt);

public:
	BT_DECLARE_ALIGNED_ALLOCATOR();

	btPlatformerController(btPairCachingGhostObject* ghostObject, btConvexShape* convexShape, btScalar stepHeight, const btVector3& up = btVector3(1.0, 0.0, 0.0));
	~btPlatformerController();

	///btActionInterface interface
	virtual void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime);

	///btActionInterface interface
	void debugDraw(btIDebugDraw* debugDrawer) {};

	void setUp(const btVector3& up);
	const btVector3& getUp() { return m_up; }

	/// This should probably be called setPositionIncrementPerSimulatorStep.
	/// This is neither a direction nor a velocity, but the amount to
	///	increment the position each simulation iteration, regardless
	///	of dt.
	/// This call will reset any velocity set by setVelocityForTimeInterval().
	virtual void setWalkDirection(const btVector3& walkDirection);

	/// Caller provides a velocity with which the character should move for
	///	the given time period.  After the time period, velocity is reset
	///	to zero.
	/// This call will reset any walk direction set by setWalkDirection().
	/// Negative time intervals will result in no motion.
	virtual void setVelocityForTimeInterval(const btVector3& velocity, btScalar timeInterval);

	virtual void setLinearVelocity(const btVector3& velocity);
	virtual btVector3 getLinearVelocity() const;

	void reset(btCollisionWorld* collisionWorld);
	void warp(const btVector3& origin);

	void preStep(btCollisionWorld* collisionWorld);
	void playerStep(btCollisionWorld* collisionWorld, btScalar dt);

	void setStepHeight(btScalar h) { m_stepHeight = h; }
	btScalar getStepHeight() const { return m_stepHeight; }

	/// The max slope determines the maximum angle that the controller can walk up.
	/// The slope angle is measured in radians.
	void setMaxSlope(btScalar slopeRadians);
	btScalar getMaxSlope() const;

	void jump(const btVector3& v);
	void jump() { jump(btVector3(0, 0, 0)); };

	bool canJump() const;
	bool onGround() const;
	void setUpInterpolate(bool value) {};

	void setGravity(const btVector3& gravity);
	btVector3 getGravity() const;

	void setCoyoteTime(btScalar value) { m_coyoteTime = value; }

	btPairCachingGhostObject* getGhostObject() { return m_ghostObject; }
};