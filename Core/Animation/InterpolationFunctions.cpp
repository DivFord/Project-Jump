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
}