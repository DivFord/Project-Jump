#include "AnimatedRenderComponent.h"

#include <unordered_map>
#include <windowFramework.h>
#include <auto_bind.h>
#include <partBundleNode.h>

#include "../Animation/CurrentAnim.h"

void AnimatedRenderComponent::get_anim_indices_recursive(std::unordered_map<std::string, int>& map, AnimDef* animDef)
{
	auto animName = animDef->get_anim_name();
	if (animName.size() > 0 && map.count(animName) == 0)
		map[animName] = get_anim_index(animName);
	for (int i = 0; i < animDef->child_count(); i++)
		get_anim_indices_recursive(map, animDef->get_child(i));
}

std::unordered_map<std::string, int> AnimatedRenderComponent::get_anim_indices(AnimDef* rootAnim)
{
	std::unordered_map<std::string, int> map;
	get_anim_indices_recursive(map, rootAnim);
	return map;
};

AnimatedRenderComponent::AnimatedRenderComponent(NodePath parentNode, WindowFramework& window, const std::string& modelFilepath, LVector3f offset) : RenderComponent()
{
	model = window.load_model(parentNode, modelFilepath);
	model.set_pos(offset);
	auto_bind(model.node(), anims, ~0);
	load_bundles(model);
};

AnimatedRenderComponent::AnimatedRenderComponent(NodePath parentNode, WindowFramework& window, ComponentDef* def)
{
	try {
		model = window.load_model(parentNode, "Assets/" + def->get_file_name() + ".bam");
		model.set_pos(def->get_vector3());
		auto_bind(model.node(), anims, ~0);
		load_bundles(model);
		auto animMap = get_anim_indices(def->get_anim_def());
		animGraph = new AnimGraph();
		animGraph->add_layer(AnimNode::load_anim_node(def->get_anim_def(), animGraph, animMap));
		for (int i = 0; i < def->weight_binding_count(); i++)
		{
			auto weightDef = def->get_weight_binding(i);
			add_weight_binding(weightDef.messageType, weightDef.weightName, weightDef.interpolation, weightDef.min, weightDef.max);
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << '\n';
	}
	catch (int e)
	{
		std::cout << "Error " << e << '\n';
	}
	catch (...)
	{
		std::cout << "Unknown exception!" << '\n';
	}
};

void AnimatedRenderComponent::update(double deltaT)
{
	float rot = model.get_h();
	if (rot != targetRotation)
	{
		float toTarget = targetRotation - rot;
		if (abs(toTarget) > 180)
			toTarget += (toTarget < 0 ? 360 : -360);
		
		rot += toTarget * fminf(1.0f, deltaT * rotationSpeed);
		model.set_h(rot);
	}

	if (animGraph)
	{
		animGraph->update(float(deltaT));

		for (int i = 0; i < anims.get_num_anims(); i++)
		{
			AnimControl* anim = anims.get_anim(i);
			if (!anim)
				continue;
			CurrentAnim currentAnim(anim);
			float influence = animGraph->get_influence(i, currentAnim);
			for (auto part : parts)
				part->set_control_effect(anim, influence);
		}
	}
};

bool AnimatedRenderComponent::handle_message(Message message)
{
	if (message.type == Message::MessageType::MOVE_DIR)
	{
		LVector3f dir(message.valueA, -message.valueB, 0);
		targetRotation = dir.signed_angle_deg(LVector3f(0, 1, 0), LVector3f(0, 0, 1));
		return true;
	}

	if (weightBindings.count(message.type) == 0)
		return false;
	WeightBinding binding = weightBindings[message.type];
	float value = InterpolationFunctions::interpolate(binding.type, binding.min, binding.max, message.valueA);
	animGraph->set_weight(binding.weightName, value);
};

int AnimatedRenderComponent::get_anim_index(const std::string& animName)
{
	for (int i = 0; i < anims.get_num_anims(); i++)
	{
		if (anims.get_anim_name(i) == animName)
			return i;
	}
	return -1;
};

void AnimatedRenderComponent::add_weight_binding(Message::MessageType messageType, AnimWeightName weightName, InterpolationFunctions::Type interpolationType, float min, float max)
{
	weightBindings[messageType] = WeightBinding(interpolationType, min, max, weightName);
}

void AnimatedRenderComponent::play_anim(const std::string& animName, bool looping, float blendWeight)
{
	if (animGraph)
		printf("Calling play_anim on AnimatedRenderComponents that have an assigned graph may produce strange results.");

	AnimControl* anim = anims.find_anim(animName);
	if (!anim)
	{
		printf(("Anim " + animName + " not found.").c_str());
		return;
	}

	if (blendWeight < 0)
		blendWeight = 0;
	else if (blendWeight > 1)
		blendWeight = 1;
	for (auto part : parts)
		part->set_control_effect(anim, blendWeight);

	if (looping)
		anim->loop(true);
	else
		anim->play();
};

void AnimatedRenderComponent::load_bundles(NodePath path)
{
	if (path.node()->is_of_type(PartBundleNode::get_class_type()))
	{
		PartBundleNode* r = DCAST(PartBundleNode, path.node());
		if (r)
		{
			for (int i = 0; i < r->get_num_bundles(); i++)
			{
				PartBundle* bundle = r->get_bundle(i);
				parts.push_back(bundle);
				bundle->set_anim_blend_flag(true);
				bundle->set_blend_type(PartBundle::BlendType::BT_normalized_linear);
			}
		}
	}

	for (int i = 0; i < path.get_num_children(); i++)
		load_bundles(path.get_child(i));
};