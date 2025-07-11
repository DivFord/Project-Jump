#pragma once

#include <vector>
#include <unordered_map>

#include <animControlCollection.h>
#include <nodePath.h>

#include "../Entities/RenderComponent.h"
#include "../Animation/AnimGraph.h"
#include "../Animation/InterpolationFunctions.h"
#include "../Data Loading/Component Defs/ComponentDef.h"

class WindowFramework;

/// <summary>
/// Implementation of RenderComponent for an animated mesh.
/// Can either use an AnimGraph to control the animations,
/// or call the play_anim method.
/// </summary>
class AnimatedRenderComponent : public RenderComponent
{
	struct WeightBinding
	{
		InterpolationFunctions::Type type;
		float min;
		float max;
		AnimWeightName weightName;

		WeightBinding() : type{ InterpolationFunctions::Type::LINEAR }, min{ 0 }, max{ 1 }, weightName{ AnimWeightName::FREE_1 } {};

		WeightBinding(InterpolationFunctions::Type type, float min, float max, AnimWeightName weightName)
			: type{ type }, min{ min }, max{ max }, weightName{ weightName } {};
	};

	NodePath model;
	float rotationSpeed = 10.0f;
	float targetRotation = 0.0f;
	AnimControlCollection anims;
	std::vector<PartBundle*> parts;
	AnimGraph* animGraph = nullptr;
	std::unordered_map<Message::MessageType, WeightBinding> weightBindings;

	void load_bundles(NodePath path);

public:
	AnimatedRenderComponent(NodePath parentNode, WindowFramework& window, const std::string& modelFilepath, LVector3f offset = LVector3f(0));
	AnimatedRenderComponent(NodePath parentNode, WindowFramework& window, ComponentDef* def);
	~AnimatedRenderComponent()
	{
		delete animGraph;
	};

	void update(double deltaT) override;
	bool handle_message(Message message) override;

	int get_anim_index(const std::string& animName);
	void add_weight_binding(Message::MessageType messageType, AnimWeightName weightName, InterpolationFunctions::Type interpolationType, float min = 0, float max = 1);

	void play_anim(const std::string& animName, bool looping = false, float blendWeight = 1);

	void set_anim_graph(AnimGraph* animGraph)
	{
		this->animGraph = animGraph;
	};
};

