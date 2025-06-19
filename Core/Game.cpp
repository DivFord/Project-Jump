#include "Game.h"

#include <pandaFramework.h>

#include "PhysicsManager.h"
#include "EntityManager.h"

Game::Game(PandaFramework& pandaFramework)
{
	physicsManager = new PhysicsManager();
	entityManager = new EntityManager();

	PT(AsyncTaskManager) task_mgr = AsyncTaskManager::get_global_ptr();
	task_mgr->add(physicsManager);
	task_mgr->add(entityManager);
}