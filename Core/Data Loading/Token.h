#pragma once

#include <string>

struct Token
{
	enum class Type
	{
		UNSET,

		CLASS_NAME,
		BRACKET,
		SEPARATOR,
		NUMBER,
		BOOLEAN,
		OPERATOR,
		STRING,
		VAR_NAM,
	};

	Type type;
	std::string value;
	int lineNumber;

	Token(Type type, std::string value, int lineNumber) : type{ type }, value{ value }, lineNumber { lineNumber+1 } {};

	static Token make_unset() { return Token(Type::UNSET, "", -1); };

	bool unset() { return type == Type::UNSET; };

	const std::string print(bool withLineNumber = false)
	{
		std::string r;
		switch (type)
		{
		case Type::UNSET:
			r = "Unset: ";
			break;
		case Type::CLASS_NAME:
			r = "Class: ";
			break;
		case Type::BRACKET:
			r = "Bracket: ";
			break;
		case Type::SEPARATOR:
			r = "Separator: ";
			break;
		case Type::NUMBER:
			r = "Number: ";
			break;
		case Type::BOOLEAN:
			r = "Boolean: ";
			break;
		case Type::OPERATOR:
			r = "Operator: ";
			break;
		case Type::STRING:
			r = "String: ";
			break;
		case Type::VAR_NAM:
			r = "Variable: ";
			break;

		default:
			return "Undefined!";
		}
		r += value;
		if (withLineNumber)
			r += " (line " + std::to_string(lineNumber) + ")";
		return r;
	};
};