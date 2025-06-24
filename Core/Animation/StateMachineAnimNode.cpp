#include "StateMachineAnimNode.h"

#include "AnimGraph.h"

StateMachineAnimNode::Transition::Transition(AnimWeightName triggerWeight, Comparator triggerComparator, float triggerValue, int connectedState, float transitionTime) :
	triggerWeight{ triggerWeight }, connectedState{ connectedState }, transitionTime{ transitionTime }
{
	switch (triggerComparator)
	{
	case Comparator::LESS_THAN:
		minValue = FLT_MIN;
		maxValue = triggerValue;
		break;

	case Comparator::LESS_THAN_EQUALS:
		minValue = FLT_MIN;
		maxValue = triggerValue + FLT_EPSILON;
		break;

	case Comparator::EQUALS:
		minValue = triggerValue - FLT_EPSILON;
		maxValue = triggerValue + FLT_EPSILON;
		break;

	case Comparator::GREATER_THAN_EQUALS:
		minValue = triggerValue - FLT_EPSILON;
		maxValue = FLT_MAX;
		break;

	case Comparator::GREATER_THAN:
		minValue = triggerValue;
		maxValue = FLT_MAX;
		break;
	}
}

StateMachineAnimNode::StateMachineAnimNode(AnimNode* startStateAnim, AnimGraph* animGraph) : AnimNode(animGraph)
{
	states.push_back(startStateAnim);
	currentState = 0;
	animGraph->register_state_machine(this);
}

float StateMachineAnimNode::get_influence(int animIndex, CurrentAnim& currentAnim)
{
	if (currentState < 0 || currentState >= states.size())
		return 0;
	if (transitionTimer <= 0 || nextState < 0 || nextState >= states.size())
		return states[currentState]->get_influence(animIndex, currentAnim);

	float currentInfluence = states[currentState]->get_influence(animIndex, currentAnim) * transitionTimer;
	float nextInfluence = states[nextState]->get_influence(animIndex, currentAnim) * (1.0f - transitionTimer);
	return currentInfluence + nextInfluence;
};

int StateMachineAnimNode::add_state(AnimNode* animNode)
{
	states.push_back(animNode);
	return states.size() - 1;
};

void StateMachineAnimNode::add_transition(int from, int to, AnimWeightName triggerWeight, Comparator triggerComparator, float triggerValue, float transitionTime)
{
	transitions[from].push_back(Transition(triggerWeight, triggerComparator, triggerValue, to, transitionTime));
};

void StateMachineAnimNode::update(float deltaT)
{
	check_triggers();
	advance_transition(deltaT);
}

void StateMachineAnimNode::check_triggers()
{
	if (nextState > 0)
		return;

	if (transitions.count(currentState) < 1)
		return;

	for (const Transition& transition : transitions[currentState])
	{
		float weight = get_weight(transition.triggerWeight);
		if (weight > transition.minValue && weight < transition.maxValue)
		{
			nextState = transition.connectedState;
			transitionTimer = 0;
			transitionSpeed = 1.0f / transition.transitionTime;
			return;
		}
	}
}

void StateMachineAnimNode::advance_transition(float deltaT)
{
	if (nextState < 0)
		return;

	transitionTimer += transitionSpeed * deltaT;

	if (transitionTimer >= 1)
	{
		transitionTimer = 0;
		transitionSpeed = 0;
		currentState = nextState;
		nextState = -1;
	}
}