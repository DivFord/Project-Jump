#pragma once

#include <unordered_map>

#include "AnimWeightName.h"
#include "CurrentAnim.h"

class AnimGraph;

/// <summary>
/// Abstract base class for nodes in an AnimGraph.
/// </summary>
class AnimNode
{
	AnimGraph* graph;

protected:
	float get_weight(AnimWeightName weightName);

public:
	AnimNode(AnimGraph* graph);

	/// <summary>
	/// Given the index of an animation clip, returns a 0-1 value indicating
	/// how strongly that clip should influence the final pose.
	/// We also pass in an interface to the animation system, in case the
	/// AnimNode wants to control playback speed or position.
	/// </summary>
	/// <param name="animIndex"></param>
	/// <param name="currentAnim"></param>
	/// <returns></returns>
	virtual float get_influence(int animIndex, CurrentAnim& currentAnim) = 0;
};