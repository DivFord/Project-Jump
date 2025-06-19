#pragma once

#include <pointerTo.h>

#include "GamePhysicsManager.h"
#include "EntityManager.h"

class PandaFramework;
class WindowFramework;

class Game
{
	PT(GamePhysicsManager) physicsManager;
	PT(EntityManager) entityManager;

public:

	Game(PandaFramework& pandaFramework, WindowFramework& windowFramework);
};

