#pragma once

#include <lvecBase2.h>
#include <string>

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

	static MessageType str_to_message(std::string string)
	{
		if (string == "MOVE_INPUT")
			return MessageType::MOVE_INPUT;
		if (string == "JUMP_INPUT")
			return MessageType::JUMP_INPUT;

		if (string == "MOVE_SPEED")
			return MessageType::MOVE_SPEED;
		if (string == "MOVE_DIR")
			return MessageType::MOVE_DIR;
		if (string == "VERT_SPEED")
			return MessageType::VERT_SPEED;
		if (string == "GROUND_DIST")
			return MessageType::GROUND_DIST;

		return MessageType::UNSET;
	};

	static std::string message_to_str(MessageType message)
	{
		if (message == MessageType::MOVE_INPUT)
			return "MOVE_INPUT";
		if (message == MessageType::JUMP_INPUT)
			return "JUMP_INPUT";

		if (message == MessageType::MOVE_SPEED)
			return "MOVE_SPEED";
		if (message == MessageType::MOVE_DIR)
			return "MOVE_DIR";
		if (message == MessageType::VERT_SPEED)
			return "VERT_SPEED";
		if (message == MessageType::GROUND_DIST)
			return "GROUND_DIST";

		if (message == MessageType::UNSET)
			return "UNSET";

		return "--ERROR--";
	}
};

