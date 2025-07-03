#pragma once

#include <ostream>
#include <lvector3.h>

struct ShapeDef
{
	enum class Type
	{
		BOX,
		CAPSULE,
		CONE,
		CYLINDER,
		SPHERE
	};

	Type type;
	float width;
	float height;
	float depth;
	LVector3f pos;

	ShapeDef();

	ShapeDef(Type type, float width, float height = 0, float depth = 0, LVector3f pos = LVector3f(0, 0, 0));
};

std::ostream& operator<<(std::ostream& os, const ShapeDef& shape);