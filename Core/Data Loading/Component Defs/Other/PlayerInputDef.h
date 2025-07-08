#pragma once

#include "../ComponentDef.h"

struct PlayerInputDef : public ComponentDef
{
	PlayerInputDef(Tokeniser& tokeniser)
	{
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() override { return ComponentDef::Type::PLAYER_INPUT; };

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "PlayerInputDef { }";
		return os;
	};
};