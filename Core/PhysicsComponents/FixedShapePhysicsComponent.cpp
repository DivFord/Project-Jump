#include "FixedShapePhysicsComponent.h"

#include <bulletRigidBodyNode.h>
#include <bulletBoxShape.h>
#include <bulletCapsuleShape.h>
#include <bulletConeShape.h>
#include <bulletCylinderShape.h>
#include <bulletSphereShape.h>

#include "../GamePhysicsManager.h"

static PT(BulletShape) make_shape(ShapeDef& shape)
{
	switch (shape.type)
	{
	case ShapeDef::Type::BOX:
		return new BulletBoxShape(LVecBase3(shape.width, shape.depth, shape.height));

	case ShapeDef::Type::CAPSULE:
		return new BulletCapsuleShape(shape.width, shape.height);

	case ShapeDef::Type::CONE:
		return new BulletConeShape(shape.width, shape.height);

	case ShapeDef::Type::CYLINDER:
		return new BulletCylinderShape(shape.width, shape.height);

	case ShapeDef::Type::SPHERE:
		return new BulletSphereShape(shape.width);

	default:
		printf("Shape type not recognised!");
		return new BulletSphereShape(1.0f);
	}
};

FixedShapePhysicsComponent::FixedShapePhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, ShapeDef shape)
{
	PT(BulletShape) bulletShape = make_shape(shape);
	PT(BulletRigidBodyNode) rigidbody = new BulletRigidBodyNode(entityName.c_str());
	rigidbody->add_shape(bulletShape, TransformState::make_pos(shape.pos));

	nodePath = render.attach_new_node(rigidbody);
	physicsManager->attach(rigidbody);
};

FixedShapePhysicsComponent::FixedShapePhysicsComponent(std::string entityName, NodePath render, PT(GamePhysicsManager) physicsManager, ComponentDef* def)
{
	//TODO: SUPPORT MULTIPLE SHAPES.
	ShapeDef shape = def->get_shape(0);
	PT(BulletShape) bulletShape = make_shape(shape);
	PT(BulletRigidBodyNode) rigidbody = new BulletRigidBodyNode(entityName.c_str());
	rigidbody->add_shape(bulletShape, TransformState::make_pos(shape.pos));

	nodePath = render.attach_new_node(rigidbody);
	physicsManager->attach(rigidbody);
};