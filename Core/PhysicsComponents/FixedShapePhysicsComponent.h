#pragma once

#include <nodePath.h>
#include <lvector3.h>

#include "../Entities/Entity.h"

class GamePhysicsManager;

struct ShapeDef
{
	enum class Type
	{
		BOX,
		CAPSULE,
		CONE,
		CYLINDER,
		SPHERE
	};

	Type type;
	float width;
	float height;
	float depth;
	LVector3f pos;

	ShapeDef(Type type, float width, float height = 0, float depth = 0, LVector3f pos = LVector3f(0,0,0))
	{
		this->type = type;
		this->width = width;
		this->height = height;
		this->depth = depth;
		this->pos = pos;
	}
};

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

