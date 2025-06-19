#include "Game.h"

#include <pandaFramework.h>

#include "PhysicsManager.h"

Game::Game(PandaFramework& pandaFramework)
{
	physicsManager = new PhysicsManager();

	PT(AsyncTaskManager) task_mgr = AsyncTaskManager::get_global_ptr();
	task_mgr->add(physicsManager);
}