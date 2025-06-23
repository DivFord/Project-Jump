#include "GamePhysicsManager.h"

#include <bulletDebugNode.h>

static const int maxSubsteps = 10;
static const double stepSize = 1.0 / 180.0;

GamePhysicsManager::GamePhysicsManager() : AsyncTask(std::string("Physics"))
{
	bulletWorld = new BulletWorld();
	bulletWorld->set_gravity(0, 0, -9.81);
};

AsyncTask::DoneStatus GamePhysicsManager::do_task()
{
	ClockObject* clock = ClockObject::get_global_clock();
	bulletWorld->do_physics(clock->get_dt(), maxSubsteps, stepSize);
	return DS_cont;
};

void GamePhysicsManager::attach(TypedObject* object)
{
	bulletWorld->attach(object);
};

LVector3 GamePhysicsManager::get_gravity()
{
	return bulletWorld->get_gravity();
};

BulletClosestHitRayResult GamePhysicsManager::ray_cast(const LPoint3f& start, const LVector3f& direction, const CollideMask& mask)
{
	return bulletWorld->ray_test_closest(start, start + direction, mask);
};

BulletClosestHitRayResult GamePhysicsManager::line_cast(const LPoint3f& start, const LPoint3f& end, const CollideMask& mask)
{
	return bulletWorld->ray_test_closest(start, end, mask);
};

BulletClosestHitSweepResult GamePhysicsManager::sweep_test(BulletShape* shape, const TransformState& from_ts, const TransformState& to_ts, const CollideMask& mask)
{
	return bulletWorld->sweep_test_closest(shape, from_ts, to_ts, mask);
};

void GamePhysicsManager::create_debug(NodePath render)
{
	PT(BulletDebugNode) debugNode = new BulletDebugNode("Debug");
	debugNode->show_wireframe(true);
	debug = render.attach_new_node(debugNode);
	debug.show();
	bulletWorld->set_debug_node(debugNode);
};