#pragma once

#include <string>

#include "../ComponentDef.h"

struct MeshRenderDef : public ComponentDef
{
	std::string fileName;

	MeshRenderDef(Tokeniser& tokeniser)
	{
		tokeniser.pass_bracket("{");
		Token current = tokeniser.get_current();
		if (current.type == Token::Type::STRING)
			fileName = current.value;
		else
			throw DataLoadingException::value_mismatch(current, "string");
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() override { return ComponentDef::Type::MESH_RENDER; };
};