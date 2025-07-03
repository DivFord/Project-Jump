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
		BAD_VALUE,
	};

	Token token;
	Type type;
	std::string message;

	DataLoadingException(Token token, Type type, std::string message = "") : token{token}, type{type}, message{message} {};

	static DataLoadingException missing_file(std::string filename) { return DataLoadingException(Token::make_unset(), Type::MISSING_FILE, filename); };
	static DataLoadingException missing_bracket(Token token) { return DataLoadingException(token, Type::MISSING_BRACKET); };
	static DataLoadingException value_mismatch(Token token, std::string expectedType) { return DataLoadingException(token, Type::VALUE_MISMATCH, expectedType); };
	static DataLoadingException invalid_operator(Token token) { return DataLoadingException(token, Type::INVALID_OPERATOR); };
	static DataLoadingException bad_value(Token token) { return DataLoadingException(token, Type::BAD_VALUE); };

	void print()
	{
		switch (type)
		{
		case Type::MISSING_FILE:
			std::cout << "Missing File Exception: " << message;
			break;

		case Type::MISSING_BRACKET:
			std::cout << "Missing Bracket Exception: " << token.print(true);
			break;

		case Type::VALUE_MISMATCH:
			std::cout << "Value Mismatch Exception: found " << token.print(true) << " but was expecting " << message;
			break;

		case Type::INVALID_OPERATOR:
			std::cout << "Invalid Operator Exception: " << token.print(true);
			break;

		case Type::BAD_VALUE:
			std::cout << "Bad Value Exception: " << token.print(true);
		}
		std::cout << '\n';
	};
};