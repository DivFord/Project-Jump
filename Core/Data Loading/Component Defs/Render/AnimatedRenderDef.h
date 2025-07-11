#pragma once

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

	ComponentDef::Type get_type() const override { return ComponentDef::Type::ANIMATED_RENDER; };
	std::string get_type_str() const override { return "AnimatedRenderDef"; };

	std::string get_file_name() const override { return fileName; };

	LVector3f get_vector3(Vector3ID id) const override
	{
		if (id == Vector3ID::POS)
			return pos;
		std::cout << "Vector3ID " << (int)id << " not recognised by AnimatedRenderDef.";
		return LVector3f(0);
	};

	AnimDef* get_anim_def() const override { return animDef; };

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