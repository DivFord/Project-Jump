#pragma once

#include <vector>

#include <animControlCollection.h>
#include <nodePath.h>

#include "../Entities/RenderComponent.h"

class WindowFramework;

class AnimatedRenderComponent : public RenderComponent
{
	NodePath model;
	AnimControlCollection anims;
	std::vector<PartBundle*> parts;

	void load_bundles(NodePath path);

public:
	AnimatedRenderComponent(NodePath parentNode, WindowFramework& window, const std::string& modelFilepath, LVector3f offset = LVector3f(0));

	void update(double deltaT) override;

	void play_anim(const std::string& animName, bool looping = false, float blendWeight = 1);
	void set_anim_speed(const std::string& animName, double rate);
	void set_anim_relative_time(const std::string& animName, double relativeTime);
	void set_anim_blend_weight(const std::string& animName, float blendWeight);
};

