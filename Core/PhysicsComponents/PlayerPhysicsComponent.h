#pragma once

#include "../Entities/PhysicsComponent.h"
#include "PlayerPhysicsDef.h"

#include <nodePath.h>

class PlatformerControllerNode;
class GamePhysicsManager;
class BulletCapsuleShape;

/// <summary>
/// Implementation of PhysicsComponent that adds a BulletCharacterController
/// for a platformer-style player character.
/// </summary>
class PlayerPhysicsComponent : public PhysicsComponent
{
	PT(GamePhysicsManager) physicsManager;
	NodePath nodePath;
	PT(PlatformerControllerNode) charControl;
	PT(BulletCapsuleShape) capsule;
	float maxSpeed;
	float acceleration;
	float decceleration;
	LVector2f currentMove;
	LVector2f targetMove;
	float gravity;
	float currentVerticalVelocity;
	float jumpInputTimer;
	float jumpInputLeeway;

	void update_move(double deltaT);

public:
	PlayerPhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, PlayerPhysicsDef def);

	NodePath get_node_path() override
	{
		return nodePath;
	};

	void update(double deltaT) override;

	bool handle_message(Message message) override;
};