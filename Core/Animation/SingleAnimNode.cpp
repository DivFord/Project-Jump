#include "SingleAnimNode.h"

SingleAnimNode::SingleAnimNode(int animIndex, AnimGraph* graph) : AnimNode(graph)
{
	animID = animIndex;
}

float SingleAnimNode::get_influence(int animIndex)
{
	if (animIndex == animID)
		return 1.0f;
	return 0.0f;
}