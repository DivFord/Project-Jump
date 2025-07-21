#pragma once

#include <string>

/// <summary>
/// A set of interpolation functions.
/// Used by AnimatedRenderComponent to define a relationship
/// between a Message and an AnimWeightName.
/// </summary>
class InterpolationFunctions
{

public:
	enum class Type
	{
		LINEAR,
		QUADRADITIC,
		CUBIC
	};

	static float interpolate(Type type, float min, float max, float value);

	static Type str_to_interpolation(std::string str);
	static std::string interpolation_to_str(Type type);
};

