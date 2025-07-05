#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Component Defs/ComponentDef.h"


struct EntityDef
{
	std::string name;
	std::unique_ptr<ComponentDef> physicsComponent;
	std::unique_ptr<ComponentDef> renderComponent;
	std::vector<std::unique_ptr<ComponentDef>> otherComponents;

	EntityDef(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream& os, const EntityDef& def);
};

