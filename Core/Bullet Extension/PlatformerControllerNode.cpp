#include "PlatformerControllerNode.h"

#include <config_bullet.h>
#include <bullet_includes.h>
#include <bullet_utils.h>
#include <bulletShape.h>
#include <luse.h>
#include <transformState.h>
#include <nodePath.h>

static const btVector3 up_vectors[3] = { btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 1.0f, 0.0f), btVector3(0.0f, 0.0f, 1.0f) };

TypeHandle PlatformerControllerNode::_type_handle;

PlatformerControllerNode::PlatformerControllerNode(BulletShape* shape, PN_stdfloat step_height, const char* name) : BulletBaseCharacterControllerNode(name) {
    // Initialise variables.
    _character = nullptr;
    _ghost = nullptr;

    // Synchronised transform
    _sync = TransformState::make_identity();
    _sync_disable = false;

    // Initial transform
    btTransform trans = btTransform::getIdentity();

    // Initialise movement
    _linear_movement_is_local = false;
    _linear_movement.set(0.0f, 0.0f, 0.0f);
    _angular_movement = 0.0f;

    // Up axis
    _up = get_default_up_axis();

    // Get convex shape (for ghost object)
    if (!shape->is_convex()) {
        bullet_cat.error() << "a convex shape is required!" << std::endl;
        return;
    }

    btConvexShape* convex = (btConvexShape*)(shape->ptr());

    // Ghost object
    _ghost = new btPairCachingGhostObject();
    _ghost->setUserPointer(this);

    _ghost->setWorldTransform(trans);
    _ghost->setInterpolationWorldTransform(trans);
    _ghost->setCollisionShape(convex);
    _ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

    // Character controller
    _character = new btPlatformerController(_ghost, convex, step_height, up_vectors[_up]);
    _character->setGravity(up_vectors[_up] * -(btScalar)9.81f);


    // Retain a pointer to the shape
    _shape = shape;

    // Default collide mask TODO set_into_collide_mask(CollideMask::all_on());
};

INLINE PlatformerControllerNode::
~PlatformerControllerNode() {

    delete _character;
    delete _ghost;
};

void PlatformerControllerNode::
set_linear_movement(const LVector3& movement, bool is_local) {
    LightMutexHolder holder(BulletWorld::get_global_lock());

    nassertv(!movement.is_nan());

    _linear_movement = movement;
    _linear_movement_is_local = is_local;
};

/**
 * Assumes the lock(bullet global lock) is held by the caller
 */
void PlatformerControllerNode::
do_sync_p2b(PN_stdfloat dt, int num_substeps) {
    // Synchronise global transform
    do_transform_changed();

    // Linear movement
    LVector3 vp = _linear_movement / (btScalar)num_substeps;

    btVector3 v;
    if (_linear_movement_is_local) {
        btTransform xform = _ghost->getWorldTransform();
        xform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
        v = xform(LVecBase3_to_btVector3(vp));
    }
    else {
        v = LVecBase3_to_btVector3(vp);
    }

    // _character->setVelocityForTimeInterval(v, dt);
    _character->setWalkDirection(v * dt);
    _angular_movement = 0.0f;
};

/**
 * Assumes the lock(bullet global lock) is held by the caller
 */
void PlatformerControllerNode::
do_sync_b2p() {

    NodePath np = NodePath::any_path((PandaNode*)this);
    LVecBase3 scale = np.get_net_transform()->get_scale();

    btTransform trans = _ghost->getWorldTransform();
    CPT(TransformState) ts = btTrans_to_TransformState(trans, scale);

    LMatrix4 m_sync = _sync->get_mat();
    LMatrix4 m_ts = ts->get_mat();

    if (!m_sync.almost_equal(m_ts)) {
        _sync = ts;
        _sync_disable = true;
        np.set_transform(NodePath(), ts);
        _sync_disable = false;
    }
};

void PlatformerControllerNode::
do_transform_changed() {

    if (_sync_disable) return;

    NodePath np = NodePath::any_path((PandaNode*)this);
    CPT(TransformState) ts = np.get_net_transform();

    LMatrix4 m_sync = _sync->get_mat();
    LMatrix4 m_ts = ts->get_mat();

    if (!m_sync.almost_equal(m_ts)) {
        _sync = ts;

        // Get translation, heading and scale
        LPoint3 pos = ts->get_pos();
        PN_stdfloat heading = ts->get_hpr().get_x();
        LVecBase3 scale = ts->get_scale();

        // Set translation
        _character->warp(LVecBase3_to_btVector3(pos));

        // Set Heading
        btMatrix3x3 m = _ghost->getWorldTransform().getBasis();
        btVector3 up = m[_up];

        m = btMatrix3x3(btQuaternion(up, deg_2_rad(heading)));

        _ghost->getWorldTransform().setBasis(m);

        // Set scale
        _shape->do_set_local_scale(scale);
    }
};

void PlatformerControllerNode::
transform_changed() {

    if (_sync_disable) return;

    LightMutexHolder holder(BulletWorld::get_global_lock());

    do_transform_changed();
};

BulletShape* PlatformerControllerNode::get_shape() const {
    return _shape;
};

bool PlatformerControllerNode::do_jump()
{
    LightMutexHolder holder(BulletWorld::get_global_lock());
    if (!_character->canJump())
        return false;
    _character->jump();
    return true;
};

void PlatformerControllerNode::set_gravity(PN_stdfloat gravity)
{
    LightMutexHolder holder(BulletWorld::get_global_lock());
    _character->setGravity(up_vectors[_up] * -(btScalar)gravity);
};

PN_stdfloat PlatformerControllerNode::get_gravity() const
{
    LightMutexHolder holder(BulletWorld::get_global_lock());
    return -(PN_stdfloat)_character->getGravity()[_up];
};

INLINE btPairCachingGhostObject* PlatformerControllerNode::get_ghost() const
{
    return _ghost;
};

INLINE btCharacterControllerInterface* PlatformerControllerNode::get_character() const
{
    return _character;
};