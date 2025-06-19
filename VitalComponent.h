#pragma once

#include "Message.h"

class Entity;

/// <summary>
/// Base class for RenderComponent and PhysicsComponent, because they
/// share functionality, but shouldn't inherit from Component.
/// </summary>
class VitalComponent
{
protected:
	Entity* entity;

	virtual void on_attach() {};

public:
	VitalComponent()
	{
		entity = nullptr;
	};

	void attach(Entity* entity)
	{
		this->entity = entity;
		on_attach();
	}

	virtual bool handle_message(Message message) { return false; }
};