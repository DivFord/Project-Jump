#pragma once

#include <vector>

#include "../../../PhysicsComponents/ShapeDef.h"
#include "../../Tokeniser.h"
#include "../../VariableLoader.h"
#include "../../DataLoadingException.h"

struct FixedShapePhysicsDef
{	
	std::vector<ShapeDef> shapes;

	FixedShapePhysicsDef(Tokeniser& tokeniser)
	{
		//Check for opening bracket.
		tokeniser.pass_bracket("{");
		Token current = tokeniser.get_current();
		//Load shape.
		while (current.type == Token::Type::CLASS_NAME && current.value == "Shape")
		{
			shapes.push_back(VariableLoader::load_shape(tokeniser));
			current = tokeniser.pass_separator();
		}
		//Check for closing bracket.
		tokeniser.pass_bracket("}", true);
	};
};