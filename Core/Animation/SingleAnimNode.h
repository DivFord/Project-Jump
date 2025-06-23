#pragma once

#include "AnimNode.h"

class SingleAnimNode : public AnimNode
{
	int animID = -1;
	bool looping = false;

public:
	SingleAnimNode(int animIndex, bool looping, AnimGraph* graph);

	float get_influence(int animIndex, CurrentAnim& currentAnim) override;
};

