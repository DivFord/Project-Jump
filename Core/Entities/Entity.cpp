#include "Entity.h"

#include "../PhysicsComponents/FixedShapePhysicsComponent.h"
#include "../PhysicsComponents/PlayerPhysicsComponent.h"

#include "../RenderComponents/AnimatedRenderComponent.h"
#include "../RenderComponents/MeshRenderComponent.h"

#include "../OtherComponents/PlayerInputComponent.h"

Entity::Entity(PhysicsComponent* physicsComponent, RenderComponent* renderComponent)
{
	this->physicsComponent = physicsComponent;
	physicsComponent->attach(this);
	this->renderComponent = renderComponent;
	renderComponent->attach(this);
};

PhysicsComponent* load_physics_component(ComponentDef* def, std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager)
{
	switch (def->get_type())
	{
	case ComponentDef::Type::FIXED_SHAPE_PHYS:
		return new FixedShapePhysicsComponent(entityName, render, physicsManager, def);

	case ComponentDef::Type::PLAYER_PHYS:
		return new PlayerPhysicsComponent(entityName, render, physicsManager, def);

	default:
		std::cout << "Did not recognise " << def->get_type_str() << " as a PhysicsComponent.";
		throw 1;
	}
};

RenderComponent* load_render_component(ComponentDef* def, NodePath parentNode, WindowFramework& window)
{
	switch (def->get_type())
	{
	case ComponentDef::Type::ANIMATED_RENDER:
		return new AnimatedRenderComponent(parentNode, window, def);

	case ComponentDef::Type::MESH_RENDER:
		return new MeshRenderComponent(parentNode, window, def);

	default:
		std::cout << "Did not recognise " << def->get_type_str() << " as a RenderComponent.";
	}
};

Component* load_other_component(ComponentDef* def, NodePath parentNode, PandaFramework& framework, WindowFramework& window)
{
	switch (def->get_type())
	{
	case ComponentDef::Type::PLAYER_INPUT:
		return new PlayerInputComponent(framework, window);
	}
};

Entity::Entity(EntityDef* def, PT(GamePhysicsManager) physicsManager, PandaFramework& framework, WindowFramework& window)
{
	physicsComponent = load_physics_component(def->physicsComponent.get(), def->name, window.get_render(), physicsManager);
	physicsComponent->attach(this);
	NodePath parent = physicsComponent->get_node_path();

	renderComponent = load_render_component(def->renderComponent.get(), parent, window);
	renderComponent->attach(this);

	for (int i = 0; i < def->otherComponents.size(); i++)
	{
		Component* component = load_other_component(def->otherComponents[i].get(), parent, framework, window);
		component->attach(this);
	}
};

Entity::~Entity()
{
	delete physicsComponent;
	delete renderComponent;
	for (Component* component : components)
		delete component;
};

void Entity::update(double deltaT)
{
	physicsComponent->update(deltaT);
	renderComponent->update(deltaT);
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
};

LVector3f Entity::get_pos() { return physicsComponent->get_node_path().get_pos(); };
void Entity::set_pos(float x, float y, float z) { return physicsComponent->get_node_path().set_pos(x, y, z); };
void Entity::set_pos(LVector3f pos) { return physicsComponent->get_node_path().set_pos(pos); };

LVector3f Entity::get_hpr() { return physicsComponent->get_node_path().get_hpr(); };
void Entity::set_hpr(float h, float p, float r) { return physicsComponent->get_node_path().set_hpr(h, p, r); };
void Entity::set_hpr(LVector3f hpr) { return physicsComponent->get_node_path().set_hpr(hpr); };