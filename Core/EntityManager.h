#pragma once

#include <vector>

#include <asyncTask.h>

#include "Entities/Entity.h"

#define SP std::shared_ptr

/// <summary>
/// An AsyncTask that maintains a list of entities, and updates them
/// every frame.
/// </summary>
class EntityManager : public AsyncTask {

	std::vector<SP<Entity>> entities;

public:
	EntityManager();
	ALLOC_DELETED_CHAIN(EntityManager);

	DoneStatus do_task() override;

	void add_entity(SP<Entity> entity);
};
