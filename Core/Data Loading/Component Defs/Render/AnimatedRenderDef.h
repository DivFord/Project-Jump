#pragma once

#include <string>
#include <vector>

#include "../ComponentDef.h"
#include "../../Other Defs/AnimDefs.h"

struct AnimatedRenderDef : public ComponentDef
{
	std::string fileName;
	LVector3f pos;
	AnimDef* animDef;
	std::vector<WeightBindingDef> weightBindings;

	AnimatedRenderDef(Tokeniser& tokeniser)
	{
		tokeniser.pass_bracket("{");
		fileName = VariableLoader::load_string(tokeniser);
		tokeniser.pass_separator();
		pos = VariableLoader::load_vector(tokeniser);
		tokeniser.pass_separator();
		animDef = AnimDef::load_anim(tokeniser);
		Token current = tokeniser.pass_separator();
		while (current.type == Token::Type::CLASS_NAME)
		{
			weightBindings.push_back(WeightBindingDef(tokeniser));
			current = tokeniser.pass_separator();
		}
		tokeniser.pass_bracket("}");
	};

	~AnimatedRenderDef()
	{
		delete animDef;
	}

	ComponentDef::Type get_type() override { return ComponentDef::Type::ANIMATED_RENDER; };

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "AnimatedRenderDef { " << fileName << '\n' << pos << '\n'
			<< animDef << '\n';
		for (int i = 0; i < weightBindings.size(); i++)
			os << weightBindings[i] << '\n';
		os << " }";
		return os;
	};
};