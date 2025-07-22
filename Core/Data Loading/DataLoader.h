#pragma once

#include <memory>
#include <string>

class EntityDef;

class DataLoader
{
	class Impl;

	std::unique_ptr<Impl> pimpl;

public:
	DataLoader();
	~DataLoader();

	EntityDef* load_entity(std::string fileName, bool storeData);
};