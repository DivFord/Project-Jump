#pragma once

#include <nodePath.h>
#include <lvector3.h>

#include "../Entities/Entity.h"
#include "../Data Loading/Other Defs/ShapeDef.h"

class GamePhysicsManager;

/// <summary>
/// An implementation of PhysicsComponent for a static body with primitive shape colliders.
/// Useful for props and terrain.
/// </summary>
class FixedShapePhysicsComponent : public PhysicsComponent
{
	NodePath nodePath;

public:
	FixedShapePhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, ShapeDef shape);

	NodePath get_node_path() override
	{
		return nodePath;
	};

	void update(double deltaT) override {};//No update needed.
};

