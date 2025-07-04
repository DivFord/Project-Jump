#pragma once

#include <string>
#include <vector>

#include "Component Defs/ComponentDef.h"


struct EntityDef
{
	std::string name;
	ComponentDef physicsComponent;
	ComponentDef renderComponent;
	std::vector<ComponentDef> otherComponents;

	EntityDef(Tokeniser& tokeniser);
};

