#include "VariableLoader.h"

#include <string.h>

#include "Tokeniser.h"
#include "DataLoadingException.h"

#include <iostream>

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
		std::cout << current.print() << " LHS: " << lhs << " PREC: " << maxPrecedence << "\n";

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

LVector3f VariableLoader::load_vector(Tokeniser& tokeniser)
{
	//Find the opening bracket.
	Token next = tokeniser.get_next(Token::Type::BRACKET);
	if (next.unset() || next.value != "(")
		throw DataLoadingException::missing_bracket(next);

};

ShapeDef VariableLoader::load_shape(Tokeniser& tokeniser)
{

};