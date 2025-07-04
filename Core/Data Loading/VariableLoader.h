#pragma once

#include <lvector3.h>

#include "Other Defs/ShapeDef.h"

class Tokeniser;

class VariableLoader
{
public:
	static bool load_bool(Tokeniser& tokeniser);
	static double load_number(Tokeniser& tokeniser);
	static LVector3f load_vector(Tokeniser& tokeniser);
	static ShapeDef load_shape(Tokeniser& tokeniser);
};

