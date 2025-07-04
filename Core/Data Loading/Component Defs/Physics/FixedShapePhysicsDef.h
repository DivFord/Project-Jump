#pragma once

#include <vector>

#include "../ComponentDef.h"
#include "../../Other Defs/ShapeDef.h"

struct FixedShapePhysicsDef : public ComponentDef
{	
	std::vector<ShapeDef> shapes;

	FixedShapePhysicsDef(Tokeniser& tokeniser)
	{
		tokeniser.pass_bracket("{");
		Token current = tokeniser.get_current();
		while (current.type == Token::Type::CLASS_NAME && current.value == "Shape")
		{
			shapes.push_back(VariableLoader::load_shape(tokeniser));
			current = tokeniser.pass_separator();
		}
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() override { return ComponentDef::Type::FIXED_SHAPE_PHYS; };
};