#pragma once

#include <string>

#include "../../Tokeniser.h"
#include "../../DataLoadingException.h"

struct MeshRenderDef
{
	std::string fileName;

	MeshRenderDef(Tokeniser& tokeniser)
	{
		//Check for opening bracket.
		tokeniser.pass_bracket("{");
		Token current = tokeniser.get_current();
		//Get file name.
		if (current.type == Token::Type::STRING)
			fileName = current.value;
		else
			throw DataLoadingException::value_mismatch(current, "string");
		//Check for closing bracket.
		tokeniser.pass_bracket("}", true);
	};
};