#pragma once

#include "../ComponentDef.h"

/// <summary>
/// POD to use as input when constructing a PlayerPhysicsComponent.
/// </summary>
struct PlayerPhysicsDef : public ComponentDef
{
	float capsuleRadius = 0.5f;
	float capsuleHeight = 1.5f;

	float stepHeight = 0.25f;

	float maxSpeed = 6.0f;
	float acceleration = 5.0f;
	float decceleration = 8.0f;

	PlayerPhysicsDef() {};
	
	PlayerPhysicsDef(Tokeniser& tokeniser)
	{
		Token current = tokeniser.pass_bracket("{");
		while (current.type == Token::Type::VAR_NAM)
		{
			std::string varName = current.value;
			current = tokeniser.get_next();
			Token nameToken = current;
			
			if (current.value != "=")
				throw DataLoadingException::value_mismatch(current, "=");
			current = tokeniser.get_next();

			if (current.type != Token::Type::NUMBER)
				throw DataLoadingException::value_mismatch(current, "number");
			float varVal = std::stof(current.value);

			if (varName == "capsuleRadius")
				capsuleRadius = varVal;
			else if (varName == "capsuleHeight")
				capsuleHeight = varVal;
			else if (varName == "stepHeight")
				stepHeight = varVal;
			else if (varName == "maxSpeed")
				maxSpeed = varVal;
			else if (varName == "acceleration")
				acceleration = varVal;
			else if (varName == "decceleration")
				decceleration = varVal;
			else
				throw DataLoadingException::bad_value(nameToken);

			current = tokeniser.pass_separator();
		}
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() override { return ComponentDef::Type::PLAYER_PHYS; };

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "PlayerPhysicsDef { ";
		os << "Capsule Radius: " << capsuleRadius;
		os << ", Capsule Height: " << capsuleHeight;
		os << ", Step Height: " << stepHeight;
		os << ", Max Speed: " << maxSpeed;
		os << ", Accel: " << acceleration;
		os << ", Deccel: " << decceleration << " }";
		return os;
	};
};