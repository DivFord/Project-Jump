#pragma once

#include <lvector3.h>

#include "Other Defs/ShapeDef.h"
#include "../Animation/AnimWeightName.h"

class Tokeniser;

class VariableLoader
{
public:
	static bool load_bool(Tokeniser& tokeniser);
	static double load_numeric_expression(Tokeniser& tokeniser);
	static float load_float(Tokeniser& tokeniser);
	static int load_int(Tokeniser& tokeniser);
	static std::string load_string(Tokeniser& tokeniser);
	static AnimWeightName load_anim_weight(Tokeniser& tokeniser);
	static LVector3f load_vector(Tokeniser& tokeniser);
	static ShapeDef load_shape(Tokeniser& tokeniser);
};

