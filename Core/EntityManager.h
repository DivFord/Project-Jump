#pragma once

#include <vector>

#include <asyncTask.h>

#include "Entities/Entity.h"

class EntityManager : public AsyncTask {

	std::vector<SP<Entity>> entities;

public:
	EntityManager();
	ALLOC_DELETED_CHAIN(EntityManager);

	DoneStatus do_task() override;

	void add_entity(SP<Entity> entity);
};
