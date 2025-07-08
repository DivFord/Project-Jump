#include "ComponentLoader.h"

#include "Component Defs/Physics/FixedShapePhysicsDef.h"
#include "Component Defs/Physics/PlayerPhysicsDef.h"

#include "Component Defs/Render/MeshRenderDef.h"
#include "Component Defs/Render/AnimatedRenderDef.h"

#include "Component Defs/Other/PlayerInputDef.h"

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

std::unique_ptr<ComponentDef> ComponentLoader::load_physics_component(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::CLASS_NAME)
		throw DataLoadingException::value_mismatch(current, "Component Name");

	if (current.value == "FixedShapePhysicsComponent")
		return std::make_unique<FixedShapePhysicsDef>(tokeniser);
	if (current.value == "PlayerPhysicsComponent")
		return std::make_unique<PlayerPhysicsDef>(tokeniser);

	throw DataLoadingException::undefined_class(current, "physics component");
}

std::unique_ptr<ComponentDef> ComponentLoader::load_render_component(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::CLASS_NAME)
		throw DataLoadingException::value_mismatch(current, "Component Name");

	if (current.value == "MeshRenderComponent")
		return std::make_unique<MeshRenderDef>(tokeniser);
	if (current.value == "AnimatedRenderComponent")
		return std::make_unique<AnimatedRenderDef>(tokeniser);

	throw DataLoadingException::undefined_class(current, "render component");
}

std::unique_ptr<ComponentDef> ComponentLoader::load_other_component(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::CLASS_NAME)
		throw DataLoadingException::value_mismatch(current, "Component Name");

	if (current.value == "PlayerInputComponent")
		return std::make_unique<PlayerInputDef>(tokeniser);

	throw DataLoadingException::undefined_class(current, "general component");
}
