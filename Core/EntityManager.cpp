#include "EntityManager.h"

#include <clockObject.h>


EntityManager::EntityManager() : AsyncTask(std::string("Entities"))
{

}

AsyncTask::DoneStatus EntityManager::do_task()
{
	ClockObject* clock = ClockObject::get_global_clock();
	float deltaT = clock->get_dt();
	for (auto entity : entities)
	{
		entity->update(deltaT);
	}
	return DS_cont;
}

void EntityManager::add_entity(SP<Entity> entity)
{
	entities.push_back(entity);
}