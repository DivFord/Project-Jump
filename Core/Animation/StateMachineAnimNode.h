#pragma once

#include "AnimNode.h"

#include <vector>
#include <unordered_map>

class StateMachineAnimNode : public AnimNode
{
public:
	enum class Comparator
	{
		LESS_THAN,
		LESS_THAN_EQUALS,
		EQUALS,
		GREATER_THAN_EQUALS,
		GREATER_THAN
	};

private:
	struct Transition
	{
		AnimWeightName triggerWeight = AnimWeightName::FREE_1;
		float minValue = FLT_MIN;
		float maxValue = 0.5f;
		int connectedState = 0;
		float transitionTime = 0.5f;

		Transition() {};

		Transition(AnimWeightName triggerWeight, Comparator triggerComparator, float triggerValue, int connectedState, float transitionTime);
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

	void update(float deltaT);
};

