#pragma once

#include <lvector3.h>

#include "../PhysicsComponents/ShapeDef.h"

class Tokeniser;

class VariableLoader
{
public:
	static double load_number(Tokeniser& tokeniser);
	static LVector3f load_vector(Tokeniser& tokeniser);
	static ShapeDef load_shape(Tokeniser& tokeniser);
};

