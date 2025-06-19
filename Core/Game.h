#pragma once

class PhysicsManager;
class EntityManager;
class PandaFramework;

class Game
{
	PhysicsManager* physicsManager;
	EntityManager* entityManager;

public:

	Game(PandaFramework& pandaFramework);
};

