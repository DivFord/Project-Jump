#include "AnimGraph.h"

void AnimGraph::add_layer(AnimNode* rootNode, float layerWeight)
{
	layers.push_back(Layer(rootNode, layerWeight));
}

float AnimGraph::get_weight(int weightIndex)
{
	if (weightMap.count(weightIndex) == 0)
		return 0.0f;
	else
		return weightMap[weightIndex];
}

float AnimGraph::get_influence(int animIndex)
{
	float influence = 0.0;
	for (auto layer : layers)
		influence += layer.weight * layer.rootNode->get_influence(animIndex);
	return influence;
}