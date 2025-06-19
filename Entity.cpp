#include "Entity.h"

#include <nodePath.h>

void Entity::update(double deltaT)
{
	physicsComponent->update(deltaT);
	for (auto component : components)
		component->update(deltaT);
};

void Entity::handle_message(Message message, bool stop_when_received)
{
	if (physicsComponent->handle_message(message) && stop_when_received)
		return;
	if (renderComponent->handle_message(message) && stop_when_received)
		return;
	for (auto component : components)
	{
		bool handled = component->handle_message(message);
		if (stop_when_received && handled)
			return;
	}
}

LVector3f Entity::get_pos() { return physicsComponent->get_node_path().get_pos(); }
void Entity::set_pos(float x, float y, float z) { return physicsComponent->get_node_path().set_pos(x, y, z); }
void Entity::set_pos(LVector3f pos) { return physicsComponent->get_node_path().set_pos(pos); }

LVector3f Entity::get_hpr() { return physicsComponent->get_node_path().get_hpr(); }
void Entity::set_hpr(float h, float p, float r) { return physicsComponent->get_node_path().set_hpr(h, p, r); };
void Entity::set_hpr(LVector3f hpr) { return physicsComponent->get_node_path().set_hpr(hpr); }