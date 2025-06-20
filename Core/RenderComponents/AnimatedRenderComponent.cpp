#include "AnimatedRenderComponent.h"

#include <windowFramework.h>
#include <auto_bind.h>
#include <partBundleNode.h>

//#include <stdio.h>

AnimatedRenderComponent::AnimatedRenderComponent(NodePath parentNode, WindowFramework& window, const std::string& modelFilepath, LVector3f offset) : RenderComponent()
{
	model = window.load_model(parentNode, modelFilepath);
	model.set_pos(offset);
	auto_bind(model.node(), anims, ~0);
	load_bundles(model);
};

void AnimatedRenderComponent::update(double deltaT)
{
	if (animGraph)
	{
		for (int i = 0; i < anims.get_num_anims(); i++)
		{
			AnimControl* anim = anims.get_anim(i);
			if (!anim)
				continue;
			float influence = animGraph->get_influence(i);
			for (auto part : parts)
				part->set_control_effect(anim, influence);

			//std::cout << anim->get_name() << " " << influence << " " << (anim->is_playing() ? "Playing" : "Not Playing") << '\n';
		}
	}
};

bool AnimatedRenderComponent::handle_message(Message message)
{
	switch (message.type)
	{
	case Message::MessageType::MOVE_SPEED:
		if (animGraph)
			animGraph->set_weight(AnimWeightName::MOVE_SPEED, message.valueA);
		return true;

	default:
		return false;
	}
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

void AnimatedRenderComponent::play_anim(const std::string& animName, bool looping, float blendWeight)
{
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

void AnimatedRenderComponent::set_anim_speed(const std::string& animName, double rate)
{
	AnimControl* anim = anims.find_anim(animName);
	if (!anim)
	{
		printf(("Anim " + animName + " not found.").c_str());
		return;
	}

	anim->set_play_rate(rate);
};

void AnimatedRenderComponent::set_anim_relative_time(const std::string& animName, double relativeTime)
{
	AnimControl* anim = anims.find_anim(animName);
	if (!anim)
	{
		printf(("Anim " + animName + " not found.").c_str());
		return;
	}

	double frame = relativeTime * anim->get_num_frames();
	anim->pose(frame);
};

void AnimatedRenderComponent::set_anim_blend_weight(const std::string& animName, float blendWeight)
{
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