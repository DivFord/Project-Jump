#include "AnimDefs.h"

#include "../DataLoadingException.h"
#include "../VariableLoader.h"

#pragma region AnimDef
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
	if (current.value == "StateMachine")
		return new StateMachineDef(tokeniser);

	throw DataLoadingException::bad_value(current);
};
#pragma endregion

#pragma region SimpleAnimDef
SimpleAnimDef::SimpleAnimDef(Tokeniser& tokeniser)
{
	tokeniser.pass_bracket("(");
	animName = VariableLoader::load_string(tokeniser);
	Token current = tokeniser.pass_separator();
	if (current.type == Token::Type::BOOLEAN)
		looping = VariableLoader::load_bool(tokeniser);
	tokeniser.pass_bracket(")");
};

std::ostream& SimpleAnimDef::output(std::ostream& os) const
{
	os << "Anim (" << animName << ", " << (looping ? "LOOPING" : "NOT LOOPING") << ")";
	return os;
};
#pragma endregion

#pragma region BlendDef
BlendDef::BlendDef(Tokeniser& tokeniser)
{
	Token current = tokeniser.pass_bracket("(");
	leftAnim = AnimDef::load_anim(tokeniser);
	current = tokeniser.pass_separator();
	rightAnim = AnimDef::load_anim(tokeniser);
	current = tokeniser.pass_separator();
	weightName = VariableLoader::load_anim_weight(tokeniser);
	tokeniser.pass_bracket(")");
};

std::ostream& BlendDef::output(std::ostream& os) const
{
	os << "Blend (\n" << leftAnim << '\n' << rightAnim << '\n' << anim_weight_to_str(weightName) << "\n)";
	return os;
};
#pragma endregion

#pragma region StateMachineDef
StateMachineDef::Transition::Transition(Tokeniser& tokeniser)
{
	tokeniser.pass_bracket("(");
	fromState = VariableLoader::load_int(tokeniser);
	tokeniser.pass_separator();
	toState = VariableLoader::load_int(tokeniser);
	tokeniser.pass_separator();
	weightName = VariableLoader::load_anim_weight(tokeniser);
	triggerComparator = tokeniser.get_current().value;
	tokeniser.advance();
	triggerValue = VariableLoader::load_float(tokeniser);
	tokeniser.pass_separator();
	transitionTime = VariableLoader::load_float(tokeniser);
	tokeniser.pass_bracket(")");
};

std::ostream& operator<<(std::ostream& os, const StateMachineDef::Transition& def)
{
	os << "Transition (" << def.fromState << "->" << def.toState << ", "
		<< anim_weight_to_str(def.weightName) << " " << def.triggerComparator << " " << def.triggerValue
		<< ", " << def.transitionTime << " seconds)";
	return os;
}

StateMachineDef::StateMachineDef(Tokeniser& tokeniser)
{
	states = new std::vector<AnimDef*>;
	transitions = new std::vector<Transition>;
	Token current = tokeniser.pass_bracket("(");
	while (current.type == Token::Type::CLASS_NAME)
	{
		if (current.value == "Transition")
			transitions->push_back(Transition(tokeniser));
		else
			states->push_back(AnimDef::load_anim(tokeniser));
		current = tokeniser.pass_separator();
	}
	tokeniser.pass_bracket(")");
};

StateMachineDef::~StateMachineDef()
{
	for (int i = 0; i < states->size(); i++) {
		delete (states->at(i));
	}
	delete states;
	delete transitions;
};

std::ostream& StateMachineDef::output(std::ostream& os) const
{
	os << "StateMachine (\n";
	for (int i = 0; i < states->size(); i++)
		os << states->at(i) << '\n';
	os << '\n';
	for (int i = 0; i < transitions->size(); i++)
		os << transitions->at(i) << '\n';
	os << ")";
	return os;
};
#pragma endregion

#pragma region WeightBinding
WeightBindingDef::WeightBindingDef(Tokeniser& tokeniser)
{
	tokeniser.pass_bracket("(");
	messageType = VariableLoader::load_message(tokeniser);
	tokeniser.pass_separator();
	weightName = VariableLoader::load_anim_weight(tokeniser);
	tokeniser.pass_bracket(")");
};

std::ostream& operator<<(std::ostream& os, const WeightBindingDef& def)
{
	os << "WeightBinding (" << Message::message_to_str(def.messageType)
		<< ", " << anim_weight_to_str(def.weightName) << ")";
	return os;
};
#pragma endregion