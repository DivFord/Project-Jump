#include "DataLoader.h"

EntityDef* DataLoader::load_entity(std::string fileName, bool storeData)
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