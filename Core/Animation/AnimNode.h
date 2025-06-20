#pragma once

#include <unordered_map>

#include "AnimWeightName.h"

class AnimGraph;

class AnimNode
{
	AnimGraph* graph;

protected:
	float get_weight(AnimWeightName weightName);

public:
	AnimNode(AnimGraph* graph);

	virtual float get_influence(int animIndex) = 0;
};