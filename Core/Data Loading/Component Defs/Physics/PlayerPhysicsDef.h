#pragma once

#include "../ComponentDef.h"

/// <summary>
/// POD to use as input when constructing a PlayerPhysicsComponent.
/// </summary>
struct PlayerPhysicsDef : public ComponentDef
{
	ShapeDef capsule;

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

			float capsuleRadius = 0.5f;
			float capsuleHeight = 1.0f;

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

			capsule = ShapeDef(ShapeDef::Type::CAPSULE, capsuleRadius, capsuleHeight);

			current = tokeniser.pass_separator();
		}
		tokeniser.pass_bracket("}");
	};

	ComponentDef::Type get_type() const override { return ComponentDef::Type::PLAYER_PHYS; };
	std::string get_type_str() const override { return "PlayerPhysicsDef"; };

	float get_float(FloatID id) const override
	{
		switch (id)
		{
		case FloatID::SPEED:
			return maxSpeed;
		case FloatID::ACCEL:
			return acceleration;
		case FloatID::DECCEL:
			return decceleration;
		case FloatID::STEP_HEIGHT:
			return stepHeight;

		default:
			return 0;
		}
	};

	int shape_count() const override { return 1; };
	ShapeDef get_shape(int index) const override { return capsule; };

	virtual std::ostream& output(std::ostream& os) const override
	{
		os << "PlayerPhysicsDef { ";
		os << "Capsule Radius: " << capsule.width;
		os << ", Capsule Height: " << capsule.height;
		os << ", Step Height: " << stepHeight;
		os << ", Max Speed: " << maxSpeed;
		os << ", Accel: " << acceleration;
		os << ", Deccel: " << decceleration << " }";
		return os;
	};
};