#include "AnimWeightName.h"

AnimWeightName str_to_anim_weight(std::string name)
{
	if (name == "FREE_1")
		return AnimWeightName::FREE_1;
	if (name == "FREE_2")
		return AnimWeightName::FREE_2;
	if (name == "FREE_3")
		return AnimWeightName::FREE_3;
	if (name == "FREE_4")
		return AnimWeightName::FREE_4;
	if (name == "FREE_5")
		return AnimWeightName::FREE_5;

	if (name == "MOVE_SPEED")
		return AnimWeightName::MOVE_SPEED;
	if (name == "VERT_SPEED")
		return AnimWeightName::VERT_SPEED;
	if (name == "GROUND_DIST")
		return AnimWeightName::GROUND_DIST;

	throw '0';
}

std::string anim_weight_to_str(AnimWeightName animWeight)
{
	if (animWeight == AnimWeightName::FREE_1)
		return "FREE_1";
	if (animWeight == AnimWeightName::FREE_2)
		return "FREE_2";
	if (animWeight == AnimWeightName::FREE_3)
		return "FREE_3";
	if (animWeight == AnimWeightName::FREE_4)
		return "FREE_4";
	if (animWeight == AnimWeightName::FREE_5)
		return "FREE_5";

	if (animWeight == AnimWeightName::MOVE_SPEED)
		return "MOVE_SPEED";
	if (animWeight == AnimWeightName::VERT_SPEED)
		return "VERT_SPEED";
	if (animWeight == AnimWeightName::GROUND_DIST)
		return "GROUND_DIST";

	return "UNDEFINED";
}