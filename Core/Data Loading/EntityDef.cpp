#include "EntityDef.h"

#include "ComponentLoader.h"

#include <iostream>

EntityDef::EntityDef(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::VAR_NAM)
		throw DataLoadingException::value_mismatch(current, "name of entity");
	name = current.value;
	std::cout << current.print() << '\n';
	
	tokeniser.pass_bracket("{");
	current = tokeniser.get_current();
	std::cout << current.print() << '\n';
	physicsComponent = ComponentLoader::load_physics_component(tokeniser);

	current = tokeniser.get_current();
	std::cout << current.print() << '\n';
	renderComponent = ComponentLoader::load_render_component(tokeniser);

	current = tokeniser.get_current();
	while (current.type == Token::Type::CLASS_NAME)
	{
		std::cout << current.print() << '\n';
		otherComponents.push_back(ComponentLoader::load_other_component(tokeniser));
		current = tokeniser.get_current();
	}

	tokeniser.pass_bracket("}", true);
};