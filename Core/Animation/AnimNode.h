#pragma once

#include <unordered_map>

class AnimGraph;

class AnimNode
{
	AnimGraph* graph;

protected:
	float get_weight(int weightIndex);

public:
	AnimNode(AnimGraph* graph);

	virtual float get_influence(int animIndex) = 0;
};