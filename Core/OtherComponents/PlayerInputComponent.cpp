#include "PlayerInputComponent.h"

void handle_key(const Event* eventPtr, void* playerInputPtr)
{
	PlayerInputComponent* playerInput = static_cast<PlayerInputComponent*>(playerInputPtr);
	playerInput->set_key(eventPtr->get_name());
};

PlayerInputComponent::PlayerInputComponent(PandaFramework& framework, WindowFramework& window) : Component()
{
	window.enable_keyboard();

	void* inputPtr = this;

	framework.define_key("space", "Jump", handle_key, inputPtr);
	framework.define_key("space-up", "StopJump", handle_key, inputPtr);
	framework.define_key("w", "Forward", handle_key, inputPtr);
	framework.define_key("w-up", "StopForward", handle_key, inputPtr);
	framework.define_key("a", "Left", handle_key, inputPtr);
	framework.define_key("a-up", "StopLeft", handle_key, inputPtr);
	framework.define_key("s", "Back", handle_key, inputPtr);
	framework.define_key("s-up", "StopBack", handle_key, inputPtr);
	framework.define_key("d", "Right", handle_key, inputPtr);
	framework.define_key("d-up", "StopRight", handle_key, inputPtr);
};

void PlayerInputComponent::update(double deltaT)
{
	send_message(Message(Message::MessageType::MOVE_INPUT, get_move()), true);
};

void PlayerInputComponent::set_key(std::string keyName)
{
	if (keyName == "space")
		send_message(Message(Message::MessageType::JUMP_INPUT, true), true);
	if (keyName == "space-up")
		send_message(Message(Message::MessageType::JUMP_INPUT, false), true);

	else if (keyName == "w")
		forward = true;
	else if (keyName == "w-up")
		forward = false;
	else if (keyName == "s")
		backward = true;
	else if (keyName == "s-up")
		backward = false;
	else if (keyName == "d")
		right = true;
	else if (keyName == "d-up")
		right = false;
	else if (keyName == "a")
		left = true;
	else if (keyName == "a-up")
		left = false;
};

LVecBase2f PlayerInputComponent::get_move()
{
	float x = 0.0f;
	float y = 0.0f;

	if (forward)
		y += 1.0f;
	if (backward)
		y -= 1.0f;

	if (right)
		x += 1.0f;
	if (left)
		x -= 1.0f;

	auto vector = LVecBase2f(x, y);
	vector.normalize();
	return vector;
};