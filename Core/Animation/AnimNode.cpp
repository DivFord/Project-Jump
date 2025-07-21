#include "AnimNode.h"
#include "AnimGraph.h"
#include "BlendAnimNode.h"
#include "SingleAnimNode.h"
#include "DrivenPoseAnimNode.h"
#include "StateMachineAnimNode.h"

AnimNode::AnimNode(AnimGraph* graph)
{
	this->graph = graph;
};

AnimNode* AnimNode::load_anim_node(AnimDef* def, AnimGraph* graph, const std::unordered_map<std::string, int>& animMapping)
{
	int animIndex = 0;
	AnimNode* leftNode = nullptr;
	AnimNode* rightNode = nullptr;
	StateMachineAnimNode* stateMachine = nullptr;
	std::cout << def->get_type_str() << " " << def->get_anim_name() << " " << animMapping.count(def->get_anim_name()) << '\n';
	switch (def->get_type())
	{
	case AnimDef::Type::SIMPLE:
		animIndex = animMapping.at(def->get_anim_name());
		return new SingleAnimNode(animIndex, def->get_looping(), graph);

	case AnimDef::Type::DRIVEN:
		animIndex = animMapping.at(def->get_anim_name());
		return new DrivenPoseAnimNode(animIndex, def->get_weight_name(), graph);

	case AnimDef::Type::BLEND:
		leftNode = load_anim_node(def->get_child(0), graph, animMapping);
		rightNode = load_anim_node(def->get_child(1), graph, animMapping);
		return new BlendAnimNode(leftNode, rightNode, def->get_weight_name(), graph);

	case AnimDef::Type::STATE_MACHINE:
		if (def->child_count() == 0)
			throw 1;
		stateMachine = new StateMachineAnimNode(load_anim_node(def->get_child(0), graph, animMapping), graph);
		for (int i = 1; i < def->child_count(); i++)
			stateMachine->add_state(load_anim_node(def->get_child(i), graph, animMapping));
		for (int i = 0; i < def->transition_count(); i++) {
			auto transition = def->get_transition(i);
			stateMachine->add_transition(transition->fromState, transition->toState, transition->weightName, transition->triggerComparator, transition->triggerValue, transition->transitionTime);
		}
		return stateMachine;

	default:
		std::cout << "Anim Def type " << def->get_type_str() << " not recognised!";
		throw 0;
	}
};

float AnimNode::get_weight(AnimWeightName weightName)
{
	if (graph)
		return graph->get_weight(weightName);
	else
		return 1.0f;
};