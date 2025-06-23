#pragma once
static class InterpolationFunctions
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

