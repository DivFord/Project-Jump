#pragma once

#include <iostream>

#include "Token.h"

struct DataLoadingException {
	enum class Type
	{
		NONE,

		MISSING_FILE,

		MISSING_BRACKET,
		VALUE_MISMATCH,
		INVALID_OPERATOR,
	};

	Token token;
	Type type;

	DataLoadingException(Token token, Type type) : token{ token }, type{ type } {};

	static DataLoadingException missing_file(Token token) { return DataLoadingException(token, Type::MISSING_FILE); };
	static DataLoadingException missing_bracket(Token token) { return DataLoadingException(token, Type::MISSING_BRACKET); };
	static DataLoadingException value_mismatch(Token token) { return DataLoadingException(token, Type::VALUE_MISMATCH); };
	static DataLoadingException invalid_operator(Token token) { return DataLoadingException(token, Type::INVALID_OPERATOR); };

	void print()
	{
		switch (type)
		{
		case Type::MISSING_FILE:
			std::cout << "Missing File Exception: " << token.value;
			break;

		case Type::MISSING_BRACKET:
			std::cout << "Missing Bracket Exception: " << token.value;
			break;

		case Type::VALUE_MISMATCH:
			std::cout << "Value Mismatch Exception: " << token.value;
			break;

		case Type::INVALID_OPERATOR:
			std::cout << "Invalid Operator Exception: " << token.value;
			break;
		}
		std::cout << '\n';
	};
};