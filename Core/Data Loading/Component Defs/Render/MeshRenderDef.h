#pragma once

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

	ComponentDef::Type get_type() const override { return ComponentDef::Type::MESH_RENDER; };
	std::string get_type_str() const override { return "MeshRenderDef"; };

	std::string get_file_name() const override { return fileName; }

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "MeshRenderDef { " << fileName << " }";
		return os;
	};
};