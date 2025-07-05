#pragma once

#include <fstream>
#include <unordered_set>
#include <queue>

#include "Token.h"

class Tokeniser
{
	std::ifstream fileStream;
	std::unordered_set<std::string> classNames;
	std::queue<Token> output;

	void process_line(const char* line, int lineNumber);

public:

	Tokeniser();

	void process_file(std::string filename);

	bool empty();
	Token get_current();
	void advance();
	bool advance_until(Token::Type tokenType, bool currentIncluded = false);
	bool advance_until(std::string tokenValue, bool currentIncluded = false);
	Token get_next();
	Token get_next(Token::Type tokenType, bool currentIncluded = false);
	Token get_next(std::string tokenValue, bool currentIncluded = false);
	Token pass_bracket(std::string bracket, bool currentIncluded = false);
	Token pass_separator();
};
