#include "BlendAnimNode.h"

#include "AnimGraph.h"

BlendAnimNode::BlendAnimNode(AnimNode* leftNode, AnimNode* rightNode, AnimWeightName weightName, AnimGraph* graph) : AnimNode(graph)
{
	this->leftNode = leftNode;
	this->rightNode = rightNode;
	this->weightName = weightName;
};

BlendAnimNode::~BlendAnimNode()
{
	delete leftNode;
	delete rightNode;
};

float BlendAnimNode::get_influence(int animIndex)
{
	float weight = get_weight(weightName);
	if (weight <= 0)
		return leftNode->get_influence(animIndex);
	else if (weight >= 1)
		return rightNode->get_influence(animIndex);
	else
		return (leftNode->get_influence(animIndex) * (1.0f - weight)) + (rightNode->get_influence(animIndex) * weight);
};