#pragma once

#include <memory>

#include "Message.h"

#define SP std::shared_ptr

class Entity;

/// <summary>
/// Base class for a component containing gameplay code, to be attached
/// to an Entity.
/// </summary>
class Component
{
protected:
	SP<Entity> entity;

	void send_message(Message message, bool stop_when_received);

public:
	Component();

	virtual void attach(SP<Entity> entity);
	virtual void update(double deltaT) = 0;
	virtual bool handle_message(Message message) { return false; };
};

