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

	void process_line(const char* line);

public:

	Tokeniser();

	void process_file(std::string filename);

	Token get_next();
};

