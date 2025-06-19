#pragma once

#include <pandaFramework.h>

#include "../Entities/Component.h"

void handle_key(const Event* eventPtr, void* playerInputPtr);

/// <summary>
/// A Component that receives player input and sends out messages
/// so that other Components can respond.
/// </summary>
class PlayerInputComponent : public Component
{
	bool forward = false;
	bool backward = false;
	bool right = false;
	bool left = false;

public:
	PlayerInputComponent(PandaFramework& framework, WindowFramework& window);

	void update(double deltaT);

	void set_key(std::string keyName);

	LVecBase2f get_move();
};

