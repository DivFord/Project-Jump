#include "DrivenPoseAnimNode.h"

DrivenPoseAnimNode::DrivenPoseAnimNode(int animIndex, AnimWeightName driverName, AnimGraph* graph) : AnimNode(graph)
{
	animID = animIndex;
	driverWeight = driverName;
};

float DrivenPoseAnimNode::get_influence(int animIndex, CurrentAnim& currentAnim)
{
	if (animIndex == animID) {
		float weight = get_weight(driverWeight);
		currentAnim.set_anim_relative_time(weight);
		return 1.0f;
	}
	return 0.0f;
};