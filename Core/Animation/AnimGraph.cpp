#include "AnimGraph.h"

#include "StateMachineAnimNode.h"

void AnimGraph::add_layer(AnimNode* rootNode, float layerWeight)
{
	layers.push_back(Layer(rootNode, layerWeight));
};

float AnimGraph::get_weight(AnimWeightName weightName)
{
	if (weightMap.count(weightName) == 0)
		return 0.0f;
	else
		return weightMap[weightName];
};

void AnimGraph::set_weight(AnimWeightName weightName, float value)
{
	weightMap[weightName] = value;
};

float AnimGraph::get_influence(int animIndex, CurrentAnim& currentAnim)
{
	float influence = 0.0;
	for (auto layer : layers)
		influence += layer.weight * layer.rootNode->get_influence(animIndex, currentAnim);
	return influence;
};

void AnimGraph::register_state_machine(StateMachineAnimNode* stateMachine)
{
	if (stateMachine != nullptr)
		stateMachines.push_back(stateMachine);
};

void AnimGraph::update(float deltaT)
{
	for (auto stateMachine : stateMachines)
		stateMachine->update(deltaT);
}