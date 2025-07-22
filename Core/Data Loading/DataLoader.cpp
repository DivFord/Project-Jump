#include "DataLoader.h"

#include <unordered_map>

#include "EntityDef.h"

#pragma region Private Implementation
class DataLoader::Impl
{
	std::unordered_map<std::string, EntityDef*> loadedEntities{};

public:
	EntityDef* load_entity(std::string fileName, bool storeData)
	{
		if (loadedEntities.count(fileName) > 0)
			return loadedEntities[fileName];

		Tokeniser tokeniser;
		tokeniser.process_file(fileName);
		EntityDef* entity = new EntityDef(tokeniser);

		if (storeData)
			loadedEntities[fileName] = entity;

		return entity;
	}
};
#pragma endregion

#pragma region Interface
DataLoader::DataLoader()
{
	pimpl = std::make_unique<Impl>();
};

DataLoader::~DataLoader() = default;

EntityDef* DataLoader::load_entity(std::string fileName, bool storeData)
{
	return pimpl->load_entity(fileName, storeData);
};
#pragma endregion