#pragma once

#include <vector>

#include <animControlCollection.h>
#include <nodePath.h>

#include "../Entities/RenderComponent.h"
#include "../Animation/AnimGraph.h"

class WindowFramework;

class AnimatedRenderComponent : public RenderComponent
{
	NodePath model;
	AnimControlCollection anims;
	std::vector<PartBundle*> parts;
	AnimGraph* animGraph;

	void load_bundles(NodePath path);

public:
	AnimatedRenderComponent(NodePath parentNode, WindowFramework& window, const std::string& modelFilepath, LVector3f offset = LVector3f(0));
	~AnimatedRenderComponent()
	{
		delete animGraph;
	};

	void update(double deltaT) override;
	bool handle_message(Message message) override;

	int get_anim_index(const std::string& animName);

	void play_anim(const std::string& animName, bool looping = false, float blendWeight = 1);
	void set_anim_speed(const std::string& animName, double rate);
	void set_anim_relative_time(const std::string& animName, double relativeTime);
	void set_anim_blend_weight(const std::string& animName, float blendWeight);

	void set_anim_graph(AnimGraph* animGraph)
	{
		this->animGraph = animGraph;
	};
};

