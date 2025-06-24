#pragma once

#include <lvecBase2.h>

/// <summary>
/// Struct for messages used for communication
/// between Components on an Entity.
/// </summary>
struct Message
{
	enum class MessageType
	{
		UNSET,

		MOVE_INPUT,
		JUMP_INPUT,

		MOVE_SPEED,
		MOVE_DIR,
		VERT_SPEED,
		GROUND_DIST,
	};

	const MessageType type;
	const float valueA;
	const float valueB;

	Message(MessageType msgType) : type{ msgType }, valueA{}, valueB{} {};
	Message(MessageType msgType, float value) : type{ msgType }, valueA{ value }, valueB{} {};
	Message(MessageType msgType, float valA, float valB) : type{ msgType }, valueA{ valA }, valueB{ valB } {};
	Message(MessageType msgType, LVecBase2f vector2) : type{ msgType }, valueA{ vector2.get_x() }, valueB{ vector2.get_y() } {};
};

