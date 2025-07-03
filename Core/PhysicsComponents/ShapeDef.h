#pragma once

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

	ShapeDef()
	{
		type = Type::SPHERE;
		width = 1;
		height = 0;
		depth = 0;
		pos = LVector3f(0, 0, 0);
	};

	ShapeDef(Type type, float width, float height = 0, float depth = 0, LVector3f pos = LVector3f(0, 0, 0))
	{
		this->type = type;
		this->width = width;
		this->height = height;
		this->depth = depth;
		this->pos = pos;
	};
};