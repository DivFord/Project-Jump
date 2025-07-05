#pragma once

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
};

