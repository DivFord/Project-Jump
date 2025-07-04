#pragma once

#include <string>
#include <unordered_set>

#include "Component Defs/ComponentDef.h"

class ComponentLoader
{
public:
	static void register_names(std::unordered_set<std::string>& names);

	static ComponentDef load_physics_component(Tokeniser& tokeniser);
	static ComponentDef load_render_component(Tokeniser& tokeniser);
	static ComponentDef load_other_component(Tokeniser& tokeniser);
};

