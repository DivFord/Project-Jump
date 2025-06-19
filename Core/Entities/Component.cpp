#include "Component.h"

#include "Entity.h"

Component::Component()
{
	this->entity = nullptr;
};

void Component::attach(SP<Entity> entity)
{
	this->entity = entity;
	entity->components.push_back(this);
};

void Component::send_message(Message message, bool stop_when_received)
{
	entity->handle_message(message, stop_when_received);
};