#include "AnimNode.h"
#include "AnimGraph.h"

AnimNode::AnimNode(AnimGraph* graph)
{
	this->graph = graph;
};

float AnimNode::get_weight(AnimWeightName weightName)
{
	if (graph)
		return graph->get_weight(weightName);
	else
		return 1.0f;
};