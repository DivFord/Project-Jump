#include "AnimDefs.h"

#include "../DataLoadingException.h"
#include "../VariableLoader.h"

AnimDef::AnimDef(Tokeniser& tokeniser)
{
	Token current = tokeniser.pass_bracket("(");
	
	if (current.type != Token::Type::STRING)
		throw DataLoadingException::value_mismatch(current, "string");
	else
		animName = current.value;
	
	looping = false;
	tokeniser.advance();
	current = tokeniser.pass_separator();
	if (current.type == Token::Type::BOOLEAN)
		looping = VariableLoader::load_bool(tokeniser);

	tokeniser.pass_bracket(")");
};

std::ostream& operator<<(std::ostream& os, const AnimDef& def)
{
	os << "Anim (" << def.animName << ", " << (def.looping ? "LOOPING" : "NOT LOOPING") << ")";
	return os;
};