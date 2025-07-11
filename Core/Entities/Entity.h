#pragma once

#include <lvector3.h>
#include <nodePath.h>
#include <windowFramework.h>

#include "../GamePhysicsManager.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "Component.h"
#include "../Data Loading/EntityDef.h"

/// <summary>
/// Class for an Entity in the game. This can be anything like a character,
/// prop, piece of level geometry, or decoration.
/// </summary>
class Entity
{
	PhysicsComponent* physicsComponent;
	RenderComponent* renderComponent;
	std::vector<Component*> components;

	friend void Component::attach(Entity* entity);

public:
	Entity(PhysicsComponent* physicsComponent, RenderComponent* renderComponent);
	Entity(EntityDef* def, PT(GamePhysicsManager) physicsManager, PandaFramework& framework, WindowFramework& window);

	~Entity();

	void update(double deltaT);
	void handle_message(Message message, bool stop_when_received);

	LVector3f get_pos();
	void set_pos(float x, float y, float z);
	void set_pos(LVector3f pos);

	LVector3f get_hpr();
	void set_hpr(float h, float p, float r);
	void set_hpr(LVector3f hpr);
};