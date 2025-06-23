#include "SingleAnimNode.h"

SingleAnimNode::SingleAnimNode(int animIndex, bool looping, AnimGraph* graph) : AnimNode(graph)
{
	animID = animIndex;
	this->looping = looping;
};

float SingleAnimNode::get_influence(int animIndex, CurrentAnim& currentAnim)
{
	if (animIndex == animID) {
		currentAnim.play_anim(looping);
		return 1.0f;
	}
	else {
		currentAnim.stop_anim();
		return 0.0f;
	}
};