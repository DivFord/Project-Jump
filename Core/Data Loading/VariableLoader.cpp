#include "VariableLoader.h"

#include <string.h>

#include "Tokeniser.h"
#include "DataLoadingException.h"

#include <iostream>

#pragma region Load Boolean
bool VariableLoader::load_bool(Tokeniser& tokeniser)
{
	Token current = tokeniser.get_current();
	if (current.type != Token::Type::BOOLEAN)
		throw DataLoadingException::value_mismatch(current, "boolean");
	if (current.value == "TRUE")
		return true;
	else if (current.value == "FALSE")
		return false;
	else
		throw DataLoadingException::bad_value(current);
}
#pragma endregion

#pragma region Load Number
static int operator_precedence(char operatorChar)
{
	if (operatorChar == '*' || operatorChar == '/')
		return 2;
	if (operatorChar == '+' || operatorChar == '-')
		return 1;
};

static double resolve_operator(char operatorChar, double lhs, double rhs)
{
	switch (operatorChar)
	{
	case '+': return lhs + rhs;
	case '-': return lhs - rhs;
	case '*': return lhs * rhs;
	case '/': return lhs / rhs;
	default: throw operatorChar;
	}
};

static double parse_numeric_expression(Tokeniser& tokeniser, int maxPrecedence)
{
	double lhs = 0;
	Token current = tokeniser.get_current();
	while (!current.unset())
	{
		//std::cout << current.print() << " LHS: " << lhs << " PREC: " << maxPrecedence << "\n";

		switch (current.type)
		{
		case Token::Type::NUMBER:
			lhs += stod(current.value);
			current = tokeniser.get_next();
			break;

		case Token::Type::OPERATOR:
			if (operator_precedence(current.value[0]) < maxPrecedence)
			{
				return lhs;
			}
			else
			{
				tokeniser.advance();
				double rhs = parse_numeric_expression(tokeniser, operator_precedence(current.value[0]));
				try {
					lhs = resolve_operator(current.value[0], lhs, rhs);
				}
				catch (...)
				{
					throw DataLoadingException::invalid_operator(current);
				}
				current = tokeniser.get_current();
			}
			break;

		case Token::Type::BRACKET:
			if (current.value == "(") {
				tokeniser.advance();
				lhs =  parse_numeric_expression(tokeniser, 0);
				current = tokeniser.get_next();
			}
			else
				return lhs;

		default:
			return lhs;
		}
	}
	return lhs;
};

double VariableLoader::load_number(Tokeniser& tokeniser)
{
	return parse_numeric_expression(tokeniser, 0);
};
#pragma endregion

#pragma region Load Vector
LVector3f VariableLoader::load_vector(Tokeniser& tokeniser)
{
	//Find the opening bracket.
	Token current = tokeniser.pass_bracket("(");
	//Find up to three numbers.
	double vals[3]{ 0 };
	int i = 0;
	while (current.value != ")" && i < 3)
	{
		if (current.type == Token::Type::SEPARATOR)
			current = tokeniser.get_next();
		else if (current.type == Token::Type::NUMBER || current.type == Token::Type::BRACKET) {
			vals[i++] = load_number(tokeniser);
			current = tokeniser.get_current();
		}
		else
			throw DataLoadingException::value_mismatch(current, "separator, bracket, or number");
	}
	//Find the closing bracket.
	tokeniser.pass_bracket(")");
	//Return!
	return LVector3f(vals[0], vals[1], vals[2]);
};
#pragma endregion

#pragma region Load Shape
ShapeDef VariableLoader::load_shape(Tokeniser& tokeniser)
{
	//Find the opening bracket.
	Token current = tokeniser.pass_bracket("(");
	//Get the shape type.
	ShapeDef::Type type = ShapeDef::Type::SPHERE;
	if (current.type == Token::Type::STRING)
	{
		if (current.value == "BOX")
			type = ShapeDef::Type::BOX;
		else if (current.value == "CYLINDER")
			type = ShapeDef::Type::CYLINDER;
		else if (current.value == "CONE")
			type = ShapeDef::Type::CONE;
		else if (current.value == "CAPSULE")
			type = ShapeDef::Type::CAPSULE;

		tokeniser.advance();
		current = tokeniser.pass_separator();
	}
	//Get the width, depth, and breadth.
	float vals[3]{ 1 };
	int i = 0;
	while (i < 3 && current.type == Token::Type::NUMBER)
	{
		vals[i++] = load_number(tokeniser);
		current = tokeniser.pass_separator();
	}
	//Get the position.
	LVector3f pos{ 0 };
	if (current.type == Token::Type::CLASS_NAME && current.value == "v3")
	{
		pos = load_vector(tokeniser);
		current = tokeniser.get_current();
	}
	//Find the closing bracket.
	tokeniser.pass_bracket(")", true);
	//Return!
	return ShapeDef(type, vals[0], vals[1], vals[2], pos);
};
#pragma endregion