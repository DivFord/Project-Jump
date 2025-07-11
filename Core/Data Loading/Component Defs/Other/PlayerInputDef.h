#pragma once

#include "../ComponentDef.h"

struct PlayerInputDef : public ComponentDef
{
	PlayerInputDef(Tokeniser& tokeniser)
	{
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() const override { return ComponentDef::Type::PLAYER_INPUT; };
	std::string get_type_str() const override { return "PlayerInputDef"; };

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "PlayerInputDef { }";
		return os;
	};
};