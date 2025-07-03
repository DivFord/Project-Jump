#include "ShapeDef.h"

ShapeDef::ShapeDef()
{
	type = Type::SPHERE;
	width = 1;
	height = 0;
	depth = 0;
	pos = LVector3f(0, 0, 0);
};

ShapeDef::ShapeDef(Type type, float width, float height, float depth, LVector3f pos)
{
	this->type = type;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->pos = pos;
};

std::ostream& operator<<(std::ostream& os, const ShapeDef& shape)
{
	switch (shape.type)
	{
	case ShapeDef::Type::BOX: os << "Box"; break;
	case ShapeDef::Type::CAPSULE: os << "Capsule"; break;
	case ShapeDef::Type::CONE: os << "Cone"; break;
	case ShapeDef::Type::CYLINDER: os << "Cylinder"; break;
	case ShapeDef::Type::SPHERE: os << "Sphere"; break;
	}
	os << ", width: " << shape.width << ", height: " << shape.height << ", depth: " << shape.depth << ", pos: " << shape.pos;
	return os;
}