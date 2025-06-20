#pragma once

#include "AnimNode.h"
#include <unordered_map>

class BlendAnimNode : public AnimNode
{
	AnimNode* leftNode;
	AnimNode* rightNode;
	int weightIndex;

public:
	BlendAnimNode(AnimNode* leftNode, AnimNode* rightNode, int weightIndex, AnimGraph* graph);
	~BlendAnimNode();

	float get_influence(int animIndex) override;
};

