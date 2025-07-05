#pragma once

#include <string>

#include "../Tokeniser.h"

struct AnimDef
{
	std::string animName;
	bool looping;

	AnimDef(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream& os, const AnimDef& def);
};