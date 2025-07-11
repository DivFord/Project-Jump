#pragma once

#include <string>
#include <unordered_map>

#include "EntityDef.h"

class DataLoader
{
	std::unordered_map<std::string, EntityDef*> loadedEntities{};

public:
	EntityDef* load_entity(std::string fileName, bool storeData);
};