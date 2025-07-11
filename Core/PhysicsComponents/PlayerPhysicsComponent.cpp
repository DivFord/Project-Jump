#include "PlayerPhysicsComponent.h"

#include "../Bullet Extension/PlatformerControllerNode.h"
#include "../GamePhysicsManager.h"
#include "../Entities/Entity.h"

#include <pandaFramework.h>
#include <bulletCapsuleShape.h>

#include <stdio.h>

const static float downRayDist = 1.0f;

PlayerPhysicsComponent::PlayerPhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, float radius, float height, float stepHeight, float speed, float accel, float deccel)
{
	this->physicsManager = physicsManager;

	capsule = new BulletCapsuleShape(radius, height);
	charControl = new PlatformerControllerNode(capsule, stepHeight, entityName.c_str());

	nodePath = render.attach_new_node(charControl);
	physicsManager->attach(charControl);

	maxSpeed = speed;
	acceleration = accel;
	decceleration = deccel;
	gravity = physicsManager->get_gravity().get_z();
}

PlayerPhysicsComponent::PlayerPhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, ComponentDef* def)
{
	this->physicsManager = physicsManager;

	ShapeDef shape = def->get_shape();
	capsule = new BulletCapsuleShape(shape.width, shape.height);
	charControl = new PlatformerControllerNode(capsule, def->get_float(ComponentDef::FloatID::STEP_HEIGHT), entityName.c_str());

	nodePath = render.attach_new_node(charControl);
	physicsManager->attach(charControl);

	maxSpeed = def->get_float(ComponentDef::FloatID::SPEED);
	acceleration = def->get_float(ComponentDef::FloatID::ACCEL);
	decceleration = def->get_float(ComponentDef::FloatID::DECCEL);
	gravity = physicsManager->get_gravity().get_z();
};

void PlayerPhysicsComponent::update(double deltaT)
{
	update_move(deltaT);
	charControl->set_linear_movement(LVector3f(currentMove.get_x(), currentMove.get_y(), 0), false);

	if (jumpInputTimer > 0)
	{
		if (charControl->do_jump())
			jumpInputTimer = 0;
		else
			jumpInputTimer -= deltaT;
	}

	auto newPos = charControl->get_transform()->get_pos();
	float speed = 0;
	float vertSpd = 0;
	if (prevPos != newPos) {
		auto move = prevPos - newPos;
		vertSpd = move.get_z();
		move.set_z(0);
		speed = (move / deltaT).length();
	}
	if (__is_nanf(speed))
		speed = 0.0f;
	prevSpeed = (prevSpeed + speed) / 2.0f;
	entity->handle_message(Message(Message::MessageType::MOVE_SPEED, prevSpeed), true);
	entity->handle_message(Message(Message::MessageType::MOVE_DIR, currentMove.normalized()), true);
	entity->handle_message(Message(Message::MessageType::VERT_SPEED, vertSpd), true);
	prevPos = newPos;

	auto downRayResult = physicsManager->ray_cast(newPos - LVector3f(0,0,capsule->get_height()), LVector3f(0, 0, -downRayDist));
	entity->handle_message(Message(Message::MessageType::GROUND_DIST, downRayResult.get_hit_fraction()), true);
};

void PlayerPhysicsComponent::update_move(double deltaT)
{
	if (currentMove == targetMove)
		return;

	float distInCurrentDir = targetMove.dot(currentMove);
	LVector2f steeringForce = targetMove - currentMove;
	if (distInCurrentDir > 0)
		currentMove += steeringForce * acceleration * deltaT;
	else
		currentMove += steeringForce * decceleration * deltaT;
};

bool PlayerPhysicsComponent::handle_message(Message message)
{
	switch (message.type)
	{
	case Message::MessageType::MOVE_INPUT:
		targetMove = LVector2(message.valueA, message.valueB) * maxSpeed;
		return true;

	case Message::MessageType::JUMP_INPUT:
		if (message.valueA > 0)
			jumpInputTimer = jumpInputLeeway;
		return true;

	default:
		return false;
	}
};