#include "PlayerPhysicsComponent.h"

#include "../Bullet Extension/PlatformerControllerNode.h"
#include "../GamePhysicsManager.h"
#include "../Entities/Entity.h"

#include <pandaFramework.h>
#include <bulletCapsuleShape.h>

#include <stdio.h>

PlayerPhysicsComponent::PlayerPhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, PlayerPhysicsDef def)
{
	this->physicsManager = physicsManager;
	capsule = new BulletCapsuleShape(def.capsuleRadius, def.capsuleHeight);
	charControl = new PlatformerControllerNode(capsule, def.stepHeight, entityName.c_str());

	nodePath = render.attach_new_node(charControl);
	physicsManager->attach(charControl);

	maxSpeed = def.maxSpeed;
	acceleration = def.acceleration;
	decceleration = def.decceleration;
	currentMove = LVector2f(0);
	targetMove = LVector2f(0);
	prevPos = LVector3(0);
	prevSpeed = 0;

	gravity = physicsManager->get_gravity().get_z();
	currentVerticalVelocity = 0;
	jumpInputTimer = 0;
	jumpInputLeeway = 0.1f;
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
	if (prevPos != newPos) {
		auto move = prevPos - newPos;
		speed = (move / deltaT).length();
	}
	if (__is_nanf(speed))
		speed = 0.0f;
	prevSpeed = (prevSpeed + speed) / 2.0f;
	entity->handle_message(Message(Message::MessageType::MOVE_SPEED, prevSpeed), true);
	prevPos = newPos;
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