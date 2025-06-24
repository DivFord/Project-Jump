#pragma once

#include "AnimNode.h"

/// <summary>
/// An implementation of AnimNode that just plays a single anim.
/// </summary>
class SingleAnimNode : public AnimNode
{
	int animID = -1;
	bool looping = false;

public:
	SingleAnimNode(int animIndex, bool looping, AnimGraph* graph);

	float get_influence(int animIndex, CurrentAnim& currentAnim) override;
};

