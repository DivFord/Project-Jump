#pragma once

#include "../Entities/PhysicsComponent.h"
#include "../Data Loading/Component Defs/ComponentDef.h"

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
	float gravity;

	LVector2f currentMove = LVector2f(0);
	LVector2f targetMove = LVector2f(0);
	LVector3f prevPos = LVector3f(0);
	float prevSpeed = 0;
	float currentVerticalVelocity = 0;
	float jumpInputTimer = 0;
	float jumpInputLeeway = 0.1f;

	void update_move(double deltaT);

public:
	PlayerPhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, float radius, float height, float stepHeight, float speed, float accel, float deccel);
	PlayerPhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, ComponentDef* def);

	NodePath get_node_path() override
	{
		return nodePath;
	};

	void update(double deltaT) override;

	bool handle_message(Message message) override;
};