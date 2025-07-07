#pragma once

#include <string>

enum class AnimWeightName
{
	FREE_1,
	FREE_2,
	FREE_3,
	FREE_4,
	FREE_5,

	MOVE_SPEED,
	VERT_SPEED,
	GROUND_DIST,
};

AnimWeightName str_to_anim_weight(std::string name);
std::string anim_weight_to_str(AnimWeightName animWeight);