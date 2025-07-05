#pragma once

#include <string>

#include "../ComponentDef.h"

struct MeshRenderDef : public ComponentDef
{
	std::string fileName;

	MeshRenderDef(Tokeniser& tokeniser)
	{
		Token current = tokeniser.pass_bracket("{");
		if (current.type == Token::Type::STRING)
			fileName = current.value;
		else
			throw DataLoadingException::value_mismatch(current, "string");
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() override { return ComponentDef::Type::MESH_RENDER; };

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "MeshRenderDef { " << fileName << " }";
		return os;
	};
};