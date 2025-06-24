#pragma once

#include "AnimNode.h"
#include <unordered_map>

/// <summary>
/// Implementation of AnimNode that has two child AnimNodes, and blends between them
/// based on a weight value.
/// </summary>
class BlendAnimNode : public AnimNode
{
	AnimNode* leftNode;
	AnimNode* rightNode;
	AnimWeightName weightName;

public:
	BlendAnimNode(AnimNode* leftNode, AnimNode* rightNode, AnimWeightName weightName, AnimGraph* graph);
	~BlendAnimNode();

	float get_influence(int animIndex, CurrentAnim& currentAnim) override;
};

