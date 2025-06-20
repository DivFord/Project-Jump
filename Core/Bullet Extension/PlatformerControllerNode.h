#pragma once

#include <pandabase.h>

#include <bulletBaseCharacterControllerNode.h>

#include "btPlatformerController.h"

class PlatformerControllerNode : public BulletBaseCharacterControllerNode
{
public:
	explicit PlatformerControllerNode(BulletShape* shape, PN_stdfloat step_height, const char* name = "Character");
	INLINE ~PlatformerControllerNode();

	void set_linear_movement(const LVector3& velocity, bool isLocal);

	BulletShape* get_shape() const;

	void set_gravity(PN_stdfloat gravity);
	PN_stdfloat get_gravity() const;

	bool do_jump();

public:
	INLINE virtual btPairCachingGhostObject* get_ghost() const;
	INLINE virtual btCharacterControllerInterface* get_character() const;

	virtual void do_sync_p2b(PN_stdfloat dt, int num_substeps);
	virtual void do_sync_b2p();

protected:
	virtual void transform_changed();

private:
	CPT(TransformState) _sync;
	bool _sync_disable;

	BulletUpAxis _up;

	btPlatformerController* _character;
	btPairCachingGhostObject* _ghost;

	PT(BulletShape) _shape;

	LVector3 _linear_movement;
	bool _linear_movement_is_local;
	PN_stdfloat _angular_movement;

	void do_transform_changed();

private:
	static TypeHandle _type_handle;

public:
	static TypeHandle get_class_type() {
		return _type_handle;
	};
	static void init_type() {
		BulletBaseCharacterControllerNode::init_type();
		register_type(_type_handle, "PlatformerControllerNode", BulletBaseCharacterControllerNode::get_class_type());
	};
	virtual TypeHandle get_type() const {
		return get_class_type();
	};
	virtual TypeHandle force_init_type() {
		init_type();
		return get_class_type();
	};
};