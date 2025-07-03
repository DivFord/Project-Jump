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
		OPERATOR,
		STRING,
		VAR_NAM,
	};

	Type type;
	std::string value;

	Token(Type type, std::string value) : type{ type }, value{ value } {};

	static Token make_unset() { return Token(Type::UNSET, ""); };

	bool unset() { return type == Type::UNSET; };

	const std::string print()
	{
		switch (type)
		{
		case Type::UNSET:
			return "Unset: " + value;
		case Type::CLASS_NAME:
			return "Class: " + value;
		case Type::BRACKET:
			return "Bracket: " + value;
		case Type::SEPARATOR:
			return "Separator: " + value;
		case Type::NUMBER:
			return "Number: " + value;
		case Type::OPERATOR:
			return "Operator: " + value;
		case Type::STRING:
			return "String: " + value;
		case Type::VAR_NAM:
			return "Variable: " + value;

		default:
			return "Undefined!";
		}
	};
};