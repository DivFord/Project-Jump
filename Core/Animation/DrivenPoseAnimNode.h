#pragma once

#include "AnimNode.h"

/// <summary>
/// An implementation of AnimNode for a single animation clip which
/// doesn't play normally, but sets its playback time using a weight variable.
/// For example, the playback position of a landing animation could be driven
/// by the distance from the ground, to ensure it always lines up correctly.
/// </summary>
class DrivenPoseAnimNode : public AnimNode
{
	int animID = -1;
	AnimWeightName driverWeight = AnimWeightName::FREE_1;

public:
	DrivenPoseAnimNode(int animIndex, AnimWeightName driverWeight, AnimGraph* graph);

	float get_influence(int animIndex, CurrentAnim& currentAnim) override;
};

