#pragma once

#include <vector>
#include <unordered_map>

#include "AnimNode.h"
#include "AnimWeightName.h"
#include "CurrentAnim.h"

class AnimGraph
{
private:
	struct Layer
	{
		AnimNode* rootNode;
		float weight;

		Layer(AnimNode* rootNode, float weight) : rootNode { rootNode }, weight{ weight } {};
	};

	std::vector<Layer> layers;
	std::unordered_map<AnimWeightName, float> weightMap;

public:
	AnimGraph() {};
	~AnimGraph()
	{
		for (auto layer : layers)
			delete layer.rootNode;
	};

	void add_layer(AnimNode* rootNode, float layerWeight = 1.0f);

	float get_weight(AnimWeightName weightName);
	void set_weight(AnimWeightName weightName, float value);

	float get_influence(int animIndex, CurrentAnim& currentAnim);
};

