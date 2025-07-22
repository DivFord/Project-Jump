#pragma once

#include <memory>

#include <asyncTask.h>
#include <bulletClosestHitRayResult.h>
#include <bulletClosestHitSweepResult.h>

class BulletShape;

/// <summary>
/// A class to set up the Bullet physics world,
/// and update it each frame.
/// </summary>
class GamePhysicsManager : public AsyncTask
{
	struct Impl;
	std::unique_ptr<Impl> pimpl;

public:
	GamePhysicsManager();
	~GamePhysicsManager();
	ALLOC_DELETED_CHAIN(GamePhysicsManager);

	DoneStatus do_task() override;

	void attach(TypedObject* object);

	LVector3 get_gravity();

	BulletClosestHitRayResult ray_cast(const LPoint3f& start, const LVector3f& direction, const CollideMask& mask = BitMask<uint32_t, 32>::all_on());
	BulletClosestHitRayResult line_cast(const LPoint3f& start, const LPoint3f& end, const CollideMask& mask = BitMask<uint32_t, 32>::all_on());
	BulletClosestHitSweepResult sweep_test(BulletShape* shape, const TransformState& from_ts, const TransformState& to_ts, const CollideMask& mask = BitMask<uint32_t, 32>::all_on());

	void create_debug(NodePath render);
};

