#include "AnimDefs.h"

#include "../DataLoadingException.h"
#include "../VariableLoader.h"

std::ostream& operator<<(std::ostream& os, const AnimDef* def)
{
	return def->output(os);
};

AnimDef* AnimDef::load_anim(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::CLASS_NAME)
		throw DataLoadingException::value_mismatch(current, "anim class");

	if (current.value == "Anim")
		return new SimpleAnimDef(tokeniser);
	if (current.value == "Blend")
		return new BlendDef(tokeniser);

	throw DataLoadingException::bad_value(current);
};

SimpleAnimDef::SimpleAnimDef(Tokeniser& tokeniser)
{
	Token current = tokeniser.pass_bracket("(");
	
	if (current.type != Token::Type::STRING)
		throw DataLoadingException::value_mismatch(current, "string");
	else
		animName = current.value;
	
	tokeniser.advance();
	current = tokeniser.pass_separator();
	if (current.type == Token::Type::BOOLEAN)
		looping = VariableLoader::load_bool(tokeniser);

	tokeniser.pass_bracket(")");
};

std::ostream& SimpleAnimDef::output(std::ostream& os) const
{
	os << "Anim (" << animName << ", " << (looping ? "LOOPING" : "NOT LOOPING") << ")";
	return os;
};

BlendDef::BlendDef(Tokeniser& tokeniser)
{
	Token current = tokeniser.pass_bracket("(");

	leftAnim = AnimDef::load_anim(tokeniser);
	current = tokeniser.pass_separator();
	rightAnim = AnimDef::load_anim(tokeniser);
	current = tokeniser.pass_separator();

	if (current.type != Token::Type::VAR_NAM)
		throw DataLoadingException::value_mismatch(current, "variable name (weight name)");
	
	try {
		weightName = str_to_anim_weight(current.value);
	}
	catch (...) {
		throw DataLoadingException::bad_value(current);
	}
};

std::ostream& BlendDef::output(std::ostream& os) const
{
	os << "Blend (\n" << leftAnim << '\n' << rightAnim << '\n' << anim_weight_to_str(weightName) << "\n)";
	return os;
};