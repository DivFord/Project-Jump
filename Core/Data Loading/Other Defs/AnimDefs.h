#pragma once

#include <string>
#include <vector>

#include "../Tokeniser.h"
#include "../../Animation/AnimWeightName.h"
#include "../../Entities/Message.h"

struct TransitionDef
{
	int fromState = 0;
	int toState = 1;
	AnimWeightName weightName = AnimWeightName::FREE_1;
	std::string triggerComparator = "=";
	float triggerValue = 1;
	float transitionTime = 0.1f;

	TransitionDef(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream& os, const TransitionDef& def);
};

struct AnimDef
{
	static AnimDef* load_anim(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream& os, const AnimDef* def);

	virtual std::string get_anim_name() const;
	virtual bool get_looping() const;
	virtual AnimWeightName get_weight_name() const;
	virtual int child_count() const;
	virtual AnimDef* get_child(int index) const;
	virtual int transition_count() const;
	virtual TransitionDef* get_transition(int index) const;


protected:
	virtual std::string get_type_str() const = 0;
	virtual std::ostream& output(std::ostream& os) const = 0;
};

struct SimpleAnimDef : public AnimDef
{
	std::string animName;
	bool looping = false;

	SimpleAnimDef(Tokeniser& tokeniser);

	virtual std::string get_anim_name() const override { return animName; };
	virtual bool get_looping() const override { return looping; };

protected:
	virtual std::string get_type_str() const override { return "SimpleAnimDef"; };
	virtual std::ostream& output(std::ostream& os) const override;
};

struct BlendDef : public AnimDef
{
	AnimDef* leftAnim;
	AnimDef* rightAnim;
	AnimWeightName weightName;

	BlendDef(Tokeniser& tokeniser);

	virtual AnimWeightName get_weight_name() const override { return weightName; };
	virtual int child_count() const override { return 2; };
	virtual AnimDef* get_child(int index) const override;

protected:
	virtual std::string get_type_str() const override { return "BlendDef"; };
	virtual std::ostream& output(std::ostream& os) const override;
};

struct StateMachineDef : public AnimDef
{
	std::vector<AnimDef*>* states;
	std::vector<TransitionDef>* transitions;

	StateMachineDef(Tokeniser& tokeniser);
	~StateMachineDef();

	virtual int child_count() const override { return states->size(); };
	virtual AnimDef* get_child(int index) const override;
	virtual int transition_count() const override { return transitions->size(); };
	virtual TransitionDef* get_transition(int index) const override;

protected:
	virtual std::string get_type_str() const override { return "StateMachineDef"; };
	virtual std::ostream& output(std::ostream& os) const override;
};

struct DrivenAnimDef : public AnimDef
{
	std::string animName;
	AnimWeightName driverWeight;

	DrivenAnimDef(Tokeniser& tokeniser);

	virtual std::string get_anim_name() const override { return animName; };
	virtual AnimWeightName get_weight_name() const override { return driverWeight; };

protected:
	virtual std::string get_type_str() const override { return "DrivenAnimDef"; };
	virtual std::ostream& output(std::ostream& os) const override;
};

struct WeightBindingDef
{
	Message::MessageType messageType = Message::MessageType::UNSET;
	AnimWeightName weightName = AnimWeightName::FREE_1;

	WeightBindingDef(Tokeniser& tokeniser);

	friend std::ostream& operator<<(std::ostream&, const WeightBindingDef& def);
};