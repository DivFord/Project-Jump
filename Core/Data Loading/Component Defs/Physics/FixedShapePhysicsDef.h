#pragma once

#include <vector>

#include "../ComponentDef.h"
#include "../../Other Defs/ShapeDef.h"

struct FixedShapePhysicsDef : public ComponentDef
{	
	std::vector<ShapeDef> shapes;

	FixedShapePhysicsDef(Tokeniser& tokeniser)
	{
		Token current = tokeniser.pass_bracket("{");
		while (current.type == Token::Type::CLASS_NAME && current.value == "Shape")
		{
			shapes.push_back(VariableLoader::load_shape(tokeniser));
			current = tokeniser.pass_separator();
		}
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() const override { return ComponentDef::Type::FIXED_SHAPE_PHYS; };
	std::string get_type_str() const override { return "FixedShapePhysicsDef"; };

	int shape_count() const override { return shapes.size(); };
	ShapeDef get_shape(int index) const override
	{
		if (index < 0 || index >= shapes.size())
		{
			std::cout << "FixedShapePhys: Index " << index << " out of bounds.";
			return ShapeDef();
		}
		return shapes[index];
	};
 
	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "FixedShapePhysicsDef {\n";
		for (int i = 0; i < shapes.size(); i++)
			os << shapes[i] << '\n';
		os << "}";
		return os;
	};
};