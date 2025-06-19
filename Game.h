#pragma once

class PhysicsManager;
class PandaFramework;

class Game
{
	PhysicsManager* physicsManager;

public:

	Game(PandaFramework& pandaFramework);
};

