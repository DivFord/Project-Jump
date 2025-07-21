#include "InterpolationFunctions.h"

float InterpolationFunctions::interpolate(Type type, float min, float max, float value)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;

	switch (type)
	{
	case Type::LINEAR:
		return value;

	case Type::QUADRADITIC:
		return value * value;

	case Type::CUBIC:
		return value * value * value;
	}
};

InterpolationFunctions::Type InterpolationFunctions::str_to_interpolation(std::string str)
{
	if (str == "LINEAR")
		return Type::LINEAR;
	if (str == "QUADRATIC")
		return Type::QUADRADITIC;
	if (str == "CUBIC")
		return Type::CUBIC;
	

	throw 0;
};

std::string InterpolationFunctions::interpolation_to_str(Type type)
{
	switch (type)
	{
	case Type::LINEAR:
		return "Linear";
	case Type::QUADRADITIC:
		return "Quadratic";
	case Type::CUBIC:
		return "Cubic";

	default:
		return "Undefined";
	}
};