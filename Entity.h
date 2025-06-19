#pragma once

#include <lvector3.h>

#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "Component.h"

#define SP std::shared_ptr


/// <summary>
/// Class for an Entity in the game. This can be anything like a character,
/// prop, piece of level geometry, or decoration.
/// </summary>
class Entity
{
	PhysicsComponent* physicsComponent;
	RenderComponent* renderComponent;
	std::vector<Component*> components;

	friend void Component::attach(SP<Entity> entity);

public:
	Entity(PhysicsComponent* physicsComponent, RenderComponent* renderComponent)
	{
		this->physicsComponent = physicsComponent;
		physicsComponent->attach(this);
		this->renderComponent = renderComponent;
		renderComponent->attach(this);
	}

	~Entity()
	{
		delete physicsComponent;
		delete renderComponent;
		for (Component* component : components)
			delete component;
	}

	void update(double deltaT);
	void handle_message(Message message, bool stop_when_received);

	LVector3f get_pos();
	void set_pos(float x, float y, float z);
	void set_pos(LVector3f pos);

	LVector3f get_hpr();
	void set_hpr(float h, float p, float r);
	void set_hpr(LVector3f hpr);
};