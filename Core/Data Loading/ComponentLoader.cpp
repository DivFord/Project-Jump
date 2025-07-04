#include "ComponentLoader.h"

#include "Component Defs/Physics/FixedShapePhysicsDef.h"

#include "Component Defs/Render/MeshRenderDef.h"

void ComponentLoader::register_names(std::unordered_set<std::string>& names)
{
	//Physics Components.
	names.insert("FixedShapePhysicsComponent");
	names.insert("PlayerPhysicsComponent");
	//Render Components.
	names.insert("AnimatedRenderComponent");
	names.insert("MeshRenderComponent");
	//Other Components.
	names.insert("PlayerInputComponent");
};

ComponentDef ComponentLoader::load_physics_component(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::CLASS_NAME)
		throw DataLoadingException::value_mismatch(current, "Component Name");

	if (current.value == "FixedShapePhysicsComponent")
		return FixedShapePhysicsDef(tokeniser);

	throw DataLoadingException::undefined_class(current, "physics component");
}

ComponentDef ComponentLoader::load_render_component(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::CLASS_NAME)
		throw DataLoadingException::value_mismatch(current, "Component Name");

	if (current.value == "MeshRenderComponent")
		return MeshRenderDef(tokeniser);

	throw DataLoadingException::undefined_class(current, "render component");
}

ComponentDef ComponentLoader::load_other_component(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::CLASS_NAME)
		throw DataLoadingException::value_mismatch(current, "Component Name");

	//if (current.value == "FixedShapePhysicsComponent")
	//	return FixedShapePhysicsDef(tokeniser);

	throw DataLoadingException::undefined_class(current, "general component");
}
