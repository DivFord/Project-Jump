#pragma once

#include <string>
#include <vector>

#include "../Tokeniser.h"
#include "../../Animation/AnimWeightName.h"
#include "../../Entities/Message.h"

struct AnimDef
{
	static AnimDef* load_anim(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream& os, const AnimDef* def);

protected:
	virtual std::ostream& output(std::ostream& os) const = 0;
};

struct SimpleAnimDef : public AnimDef
{
	std::string animName;
	bool looping = false;

	SimpleAnimDef(Tokeniser& tokeniser);

	virtual std::ostream& output(std::ostream& os) const override;
};

struct BlendDef : public AnimDef
{
	AnimDef* leftAnim;
	AnimDef* rightAnim;
	AnimWeightName weightName;

	BlendDef(Tokeniser& tokeniser);

	virtual std::ostream& output(std::ostream& os) const override;
};

struct StateMachineDef : public AnimDef
{
	struct Transition
	{
		int fromState = 0;
		int toState = 1;
		AnimWeightName weightName = AnimWeightName::FREE_1;
		std::string triggerComparator = "=";
		float triggerValue = 1;
		float transitionTime = 0.1f;

		Transition(Tokeniser& tokeniser);

		friend std::ostream& operator<<(std::ostream& os, const Transition& def);
	};

	std::vector<AnimDef*>* states;
	std::vector<Transition>* transitions;

	StateMachineDef(Tokeniser& tokeniser);
	~StateMachineDef();

	virtual std::ostream& output(std::ostream& os) const override;
};

struct WeightBindingDef
{
	Message::MessageType messageType = Message::MessageType::UNSET;
	AnimWeightName weightName = AnimWeightName::FREE_1;

	WeightBindingDef(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream&, const WeightBindingDef& def);
};