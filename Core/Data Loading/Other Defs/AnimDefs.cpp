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
	if (current.value == "Driven")
		return new DrivenAnimDef(tokeniser);

	throw DataLoadingException::bad_value(current);
};

std::string AnimDef::get_anim_name() const
{
	return "";
};

bool AnimDef::get_looping() const
{
	std::cout << "The get_looping method is not implemented on " << get_type_str() << "!";
	return false;
};

AnimWeightName AnimDef::get_weight_name() const
{
	std::cout << "The get_weight_name method is not implemented on " << get_type_str() << "!";
	return AnimWeightName::FREE_1;
};

int AnimDef::child_count() const { return 0; };
AnimDef* AnimDef::get_child(int index) const
{
	std::cout << "The get_child method is not implemented on " << get_type_str() << "!";
	return nullptr;
};

int AnimDef::transition_count() const { return 0; };
TransitionDef* AnimDef::get_transition(int index) const
{
	std::cout << "The get_transition method is not implemented on " << get_type_str() << "!";
	return nullptr;
}
#pragma endregion

#pragma region SimpleAnimDef
SimpleAnimDef::SimpleAnimDef(Tokeniser& tokeniser)
{
	tokeniser.pass_bracket("(");
	animName = VariableLoader::load_string(tokeniser);
	Token current = tokeniser.pass_separator();
	if (current.type == Token::Type::BOOLEAN)
		looping = VariableLoader::load_bool(tokeniser);
	tokeniser.pass_bracket(")", true);
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
	tokeniser.pass_bracket("(");
	leftAnim = AnimDef::load_anim(tokeniser);
	tokeniser.pass_separator();
	rightAnim = AnimDef::load_anim(tokeniser);
	tokeniser.pass_separator();
	weightName = VariableLoader::load_anim_weight(tokeniser);
	tokeniser.pass_bracket(")", true);
};

AnimDef* BlendDef::get_child(int index) const
{
	if (index == 0)
		return leftAnim;
	if (index == 1)
		return rightAnim;
	std::cout << "Blend Def only has two children. Index " << index << " is not valid.";
	return nullptr;
};

std::ostream& BlendDef::output(std::ostream& os) const
{
	os << "Blend (\n" << leftAnim << '\n' << rightAnim << '\n' << anim_weight_to_str(weightName) << "\n)";
	return os;
};
#pragma endregion

#pragma region TransitionDef
TransitionDef::TransitionDef(Tokeniser& tokeniser)
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
	tokeniser.pass_bracket(")", true);
};

std::ostream& operator<<(std::ostream& os, const TransitionDef& def)
{
	os << "Transition (" << def.fromState << "->" << def.toState << ", "
		<< anim_weight_to_str(def.weightName) << " " << def.triggerComparator << " " << def.triggerValue
		<< ", " << def.transitionTime << " seconds)";
	return os;
}
#pragma endregion

#pragma region StateMachineDef
StateMachineDef::StateMachineDef(Tokeniser& tokeniser)
{
	states = new std::vector<AnimDef*>;
	transitions = new std::vector<TransitionDef>;
	Token current = tokeniser.pass_bracket("(");
	while (current.type == Token::Type::CLASS_NAME)
	{
		std::cout << current.value << '\n';
		if (current.value == "Transition")
			transitions->push_back(TransitionDef(tokeniser));
		else
			states->push_back(AnimDef::load_anim(tokeniser));
		current = tokeniser.pass_separator();
	}
	tokeniser.pass_bracket(")", true);
};

StateMachineDef::~StateMachineDef()
{
	for (int i = 0; i < states->size(); i++) {
		delete (states->at(i));
	}
	delete states;
	delete transitions;
};

AnimDef* StateMachineDef::get_child(int index) const
{
	if (index < 0 || index >= states->size())
	{
		std::cout << "Index " << index << " is out of bounds for StateMachineDef states.";
		return nullptr;
	}
	return states->at(index);
};

TransitionDef* StateMachineDef::get_transition(int index) const
{
	if (index < 0 || index >= transitions->size())
	{
		std::cout << "Index " << index << " is out of bounds for StateMachineDef transitions.";
		return nullptr;
	}
	return &(transitions->at(index));
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

#pragma region DrivenAnim
DrivenAnimDef::DrivenAnimDef(Tokeniser& tokeniser)
{
	tokeniser.pass_bracket("(");
	animName = VariableLoader::load_string(tokeniser);
	tokeniser.pass_separator();
	driverWeight = VariableLoader::load_anim_weight(tokeniser);
	tokeniser.pass_bracket(")");
}

std::ostream& DrivenAnimDef::output(std::ostream& os) const
{
	os << "Driven (" << animName << ", " << anim_weight_to_str(driverWeight) << ")";
	return os;
}
#pragma endregion

#pragma region WeightBinding
WeightBindingDef::WeightBindingDef(Tokeniser& tokeniser)
{
	tokeniser.pass_bracket("(");
	messageType = VariableLoader::load_message(tokeniser);
	
	tokeniser.pass_separator();
	weightName = VariableLoader::load_anim_weight(tokeniser);
	
	Token current = tokeniser.pass_separator();
	if (current.type == Token::Type::VAR_NAM)
		interpolation = VariableLoader::load_interpolation(tokeniser);
	
	current = tokeniser.pass_separator();
	if (current.type == Token::Type::NUMBER)
		min = VariableLoader::load_float(tokeniser);
	
	current = tokeniser.pass_separator();
	if (current.type == Token::Type::NUMBER)
		max = VariableLoader::load_float(tokeniser);

	tokeniser.pass_bracket(")", true);
};

std::ostream& operator<<(std::ostream& os, const WeightBindingDef& def)
{
	os << "WeightBinding (" << Message::message_to_str(def.messageType)
		<< ", " << anim_weight_to_str(def.weightName) << ", "
		<< InterpolationFunctions::interpolation_to_str(def.interpolation) << ", "
		<< def.min << ", " << def.max << ")";
	return os;
};
#pragma endregion