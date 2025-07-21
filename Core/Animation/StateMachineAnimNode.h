#pragma once

#include "AnimNode.h"
#include "Comparator.h"

#include <vector>
#include <unordered_map>

/// <summary>
/// An implementation of AnimNode that contains a State Machine
/// where each state is another anim node, and trigger conditions
/// based on AnimWeights cause states to crossfade to other states.
/// </summary>
class StateMachineAnimNode : public AnimNode
{
	struct Transition
	{
		AnimWeightName triggerWeight = AnimWeightName::FREE_1;
		ComparisonWindow comparison{};
		int connectedState = 0;
		float transitionTime = 0.5f;

		Transition() {};

		Transition(AnimWeightName triggerWeight, Comparator triggerComparator, float triggerValue, int connectedState, float transitionTime);
		Transition(AnimWeightName triggerWeight, std::string triggerComparator, float triggerValue, int connectedState, float transitionTime);
	};

	std::vector<AnimNode*> states;
	std::unordered_map<int, std::vector<Transition>> transitions;
	int currentState = -1;
	int nextState = -1;
	float transitionSpeed = 0;
	float transitionTimer = 0;

	void check_triggers();
	void advance_transition(float deltaT);

public:
	StateMachineAnimNode(AnimNode* startStateAnim, AnimGraph* animGraph);

	float get_influence(int animIndex, CurrentAnim& currentAnim) override;

	int add_state(AnimNode* animNode);
	void add_transition(int from, int to, AnimWeightName triggerWeight, Comparator triggerComparator, float triggerValue, float transitionTime = 0.5f);
	void add_transition(int from, int to, AnimWeightName triggerWeight, std::string triggerComparator, float triggerValue, float transitionTime = 0.5f);

	void update(float deltaT);
};

