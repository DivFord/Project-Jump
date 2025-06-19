#pragma once

struct PlayerPhysicsDef
{
	float capsuleRadius = 0.5f;
	float capsuleHeight = 1.5f;

	float stepHeight = 0.25f;

	float maxSpeed = 6.0f;
	float acceleration = 5.0f;
	float decceleration = 8.0f;
};