#pragma once

#include <string>

#include <windowFramework.h>
#include <nodePath.h>

#include "../Entities/RenderComponent.h"
#include "../Data Loading/Component Defs/ComponentDef.h"

/// <summary>
/// An implementation of RenderComponent that adds a 
/// non-animated model.
/// </summary>
class MeshRenderComponent : public RenderComponent
{
	NodePath model;

public:
	MeshRenderComponent(NodePath parentNode, WindowFramework& window, std::string modelFilepath, float scale = 1.0f);
	MeshRenderComponent(NodePath parentNode, WindowFramework& window, ComponentDef* def);

	void update(double deltaT) {};
};