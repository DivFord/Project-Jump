#pragma once

#include "AnimNode.h"

class SingleAnimNode : public AnimNode
{
	int animID = -1;

public:
	SingleAnimNode(int animIndex, AnimGraph* graph);

	float get_influence(int animIndex) override;
};

