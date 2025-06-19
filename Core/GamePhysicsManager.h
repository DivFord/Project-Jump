#pragma once

#include <asyncTask.h>
#include <bulletWorld.h>

/// <summary>
/// A class to set up the Bullet physics world,
/// and update it each frame.
/// </summary>
class GamePhysicsManager : public AsyncTask
{
	PT(BulletWorld) bulletWorld;
	NodePath debug;

public:
	GamePhysicsManager();
	ALLOC_DELETED_CHAIN(GamePhysicsManager);

	DoneStatus do_task() override;

	void attach(TypedObject* object);

	LVector3 get_gravity();

	BulletClosestHitSweepResult sweep_test(BulletShape* shape, const TransformState& from_ts, const TransformState& to_ts, const CollideMask& mask = BitMask<uint32_t, 32>::all_on());

	void create_debug(NodePath render);
};

