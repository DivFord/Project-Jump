#include "AnimNode.h"
#include "AnimGraph.h"

AnimNode::AnimNode(AnimGraph* graph)
{
	this->graph = graph;
}

float AnimNode::get_weight(int weightIndex)
{
	graph->get_weight(weightIndex);
}