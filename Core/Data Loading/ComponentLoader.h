#pragma once

#include <string>
#include <unordered_set>
#include <memory>

#include "Component Defs/ComponentDef.h"

class ComponentLoader
{
public:
	static void register_names(std::unordered_set<std::string>& names);

	static std::unique_ptr<ComponentDef> load_physics_component(Tokeniser& tokeniser);
	static std::unique_ptr<ComponentDef> load_render_component(Tokeniser& tokeniser);
	static std::unique_ptr<ComponentDef> load_other_component(Tokeniser& tokeniser);
};

