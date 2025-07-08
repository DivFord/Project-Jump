#pragma once

#include <string>

#include "../ComponentDef.h"

struct MeshRenderDef : public ComponentDef
{
	std::string fileName;

	MeshRenderDef(Tokeniser& tokeniser)
	{
		tokeniser.pass_bracket("{");
		fileName = VariableLoader::load_string(tokeniser);
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() override { return ComponentDef::Type::MESH_RENDER; };

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "MeshRenderDef { " << fileName << " }";
		return os;
	};
};