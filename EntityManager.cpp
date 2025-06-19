#include "EntityManager.h"

#include <clockObject.h>

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