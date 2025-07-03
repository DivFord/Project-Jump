#pragma once

#include "../../../PhysicsComponents/ShapeDef.h"
#include "../../Tokeniser.h"
#include "../../VariableLoader.h"
#include "../../DataLoadingException.h"

struct FixedShapePhysicsDef
{	
	ShapeDef shape;

	FixedShapePhysicsDef(Tokeniser& tokeniser)
	{
		//Check for opening bracket.
		Token next = tokeniser.get_next();
		if (next.type != Token::Type::BRACKET || next.value != "(")
			throw (DataLoadingException::missing_bracket(next));
		//Load shape.
		shape = VariableLoader::load_shape(tokeniser);
		//Check for closing bracket.
		next = tokeniser.get_next();
		if (next.type != Token::Type::BRACKET || next.value != ")")
			throw (DataLoadingException::missing_bracket(next));
	};
};