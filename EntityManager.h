#pragma once

#include <asyncTask.h>
#include <vector>

#include "Entity.h"

/// <summary>
/// A class to store all our entities, and update them each frame.
/// </summary>
class EntityManager : public AsyncTask
{
	std::vector<SP<Entity>> entities;

public:
	EntityManager() : AsyncTask("EntityManager") {}
	ALLOC_DELETED_CHAIN(EntityManager);

	void add_entity(SP<Entity> entity)
	{
		entities.push_back(entity);
	};

	DoneStatus do_task() override;
};

