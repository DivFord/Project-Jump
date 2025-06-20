#pragma once

#include "VitalComponent.h"

class NodePath;

/// <summary>
/// Base class for components that add physics behaviour to an Entity.
/// Every entity gets one of these. It also creates the parent node that
/// models, etc, are attached to.
/// </summary>
class PhysicsComponent : public VitalComponent
{
public:
	PhysicsComponent() : VitalComponent() {};

	virtual NodePath get_node_path() = 0;
};