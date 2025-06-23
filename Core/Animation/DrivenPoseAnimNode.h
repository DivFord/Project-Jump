#pragma once

#include "AnimNode.h"

class DrivenPoseAnimNode : public AnimNode
{
	int animID = -1;
	AnimWeightName driverWeight = AnimWeightName::FREE_1;

public:
	DrivenPoseAnimNode(int animIndex, AnimWeightName driverWeight, AnimGraph* graph);

	float get_influence(int animIndex, CurrentAnim& currentAnim) override;
};

