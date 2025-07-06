#pragma once

#include <string>

#include "../Tokeniser.h"

struct AnimDef
{
	static AnimDef* load_anim(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream& os, const AnimDef* def);

protected:
	virtual std::ostream& output(std::ostream& os) const = 0;
};

struct SimpleAnimDef : public AnimDef
{
	std::string animName;
	bool looping = false;

	SimpleAnimDef(Tokeniser& tokeniser);

	virtual std::ostream& output(std::ostream& os) const override;
};

struct BlendDef : public AnimDef
{
	AnimDef* leftAnim;
	AnimDef* rightAnim;
	std::string weightName;

	BlendDef(Tokeniser& tokeniser);

	virtual std::ostream& output(std::ostream& os) const override;
};