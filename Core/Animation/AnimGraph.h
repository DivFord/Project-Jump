#pragma once

#include <vector>
#include <unordered_map>

#include "AnimNode.h";

class AnimGraph
{
	struct Layer
	{
		AnimNode* rootNode;
		float weight;

		Layer(AnimNode* rootNode, float weight) : rootNode { rootNode }, weight{ weight } {};
	};

	std::vector<Layer> layers;
	std::unordered_map<int, float> weightMap;

public:
	AnimGraph() {};
	~AnimGraph()
	{
		for (auto layer : layers)
			delete layer.rootNode;
	}

	void add_layer(AnimNode* rootNode, float layerWeight = 1.0f);

	float get_weight(int weightIndex);
	float get_influence(int animIndex);
};

