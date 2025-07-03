#include "Tokeniser.h"

#include <iostream>

#include "DataLoadingException.h"

const std::string dataPath = "Data/";
const int bufferSize = 100;

#pragma region Constructor
Tokeniser::Tokeniser()
{
	//Physics Components.
	classNames.insert("FixedShapePhysicsComponent");
	classNames.insert("PlayerPhysicsComponent");
	//Render Components.
	classNames.insert("AnimatedRenderComponent");
	classNames.insert("MeshRenderComponent");
	//Other Components.
	classNames.insert("PlayerInputComponent");
	//Other Classes.
	classNames.insert("Anim");
	classNames.insert("AnimGraph");
	classNames.insert("Blend");
	classNames.insert("Driven");
	classNames.insert("StateMachine");
	classNames.insert("Transition");
	classNames.insert("v3");
	classNames.insert("WeightBind");
};
#pragma endregion

#pragma region File Processing
void Tokeniser::process_file(std::string filename)
{
	//Clear the output queue.
	while (!output.empty())
		output.pop();
	//Open the file.
	fileStream.open(dataPath + filename, std::ifstream::in);
	//Read the file line by line.
	char buffer[bufferSize];
	int lineNumber = 0;
	while (fileStream.good())
	{
		fileStream.getline(buffer, bufferSize);
		process_line(buffer, lineNumber++);
	}
};

inline static bool char_is_number(char character)
{
	return (character == '.' || (character >= '0' && character <= '9'));
};

inline static bool char_breaks_string(char character, bool inQuotes = true)
{
	return (character == ' ' || character == '(' || character == ')' || character == '{' || character == '}'
		|| character == '\0' || (!inQuotes && character == ','));
};

void Tokeniser::process_line(const char* line, int lineNumber)
{
	while (true)
	{
		char current = *line;

		if (current == '\0' || current == '#')
			break;

		if (current == ' ' || current == '\t')
		{
			line++;
			continue;
		}

		if (current == '(' || current == ')' || current == '{' || current == '}')
		{
			output.push(Token(Token::Type::BRACKET, std::string(line, 1), lineNumber));
			line++;
		}

		else if (current == ',')
		{
			output.push(Token(Token::Type::SEPARATOR, std::string(line, 1), lineNumber));
			line++;
		}

		else if (current == '=' || current == '+' || current == '*' || current == '/')
		{
			output.push(Token(Token::Type::OPERATOR, std::string(line, 1), lineNumber));
			line++;
		}

		else if (current == '-' || char_is_number(current))
		{
			const char* next = line + 1;
			while (char_is_number(*next))
				next++;
			if (next - line == 1 && current == '-') {
				output.push(Token(Token::Type::OPERATOR, std::string(line, 1), lineNumber));
				line++;
			}
			else {
				output.push(Token(Token::Type::NUMBER, std::string(line, next - line), lineNumber));
				line = next;
			}
		}

		else if (current == '"')
		{
			const char* next = line + 1;
			while (*next != '"' && *next != '\0')
				next++;
			if (*next == '"')
			{
				output.push(Token(Token::Type::STRING, std::string(line + 1, next - line - 1), lineNumber));
				line = next + 1;
			}
			else
			{
				output.push(Token(Token::Type::STRING, std::string(line + 1, next - line), lineNumber));
				line = next;
			}
		}

		else {
			const char* next = line + 1;
			while (!char_breaks_string(*next, false))
				next++;
			std::string val(line, next - line);
			if (classNames.count(val) > 0)
				output.push(Token(Token::Type::CLASS_NAME, val, lineNumber));
			else
				output.push(Token(Token::Type::VAR_NAM, val, lineNumber));
			line = next;
		}
	}
};
#pragma endregion

#pragma region Access
bool Tokeniser::empty()
{
	return output.empty();
};

Token Tokeniser::get_current()
{
	if (output.empty())
		return Token::make_unset();
	return output.front();
};

void Tokeniser::advance()
{
	if (!output.empty())
		output.pop();
};

bool Tokeniser::advance_until(Token::Type tokenType)
{
	while (!output.empty())
	{
		if (get_current().type == tokenType)
			return true;
		advance();
	}
	return false;
};

bool Tokeniser::advance_until(std::string tokenValue)
{
	while (!output.empty())
	{
		if (get_current().value == tokenValue)
			return true;
		advance();
	}
	return false;
};

Token Tokeniser::get_next()
{
	advance();
	return get_current();
};

Token Tokeniser::get_next(Token::Type tokenType)
{
	if (!advance_until(tokenType))
		return Token::make_unset();
	return get_current();
};

Token Tokeniser::get_next(std::string tokenValue)
{
	if (!advance_until(tokenValue))
		return Token::make_unset();
	return get_current();
};

void Tokeniser::pass_bracket(std::string bracket)
{
	Token next = get_next(bracket);
	if (next.unset())
		throw DataLoadingException::missing_bracket(next);
	advance();
};

Token Tokeniser::pass_separator()
{
	Token next = get_next();
	while (next.type == Token::Type::SEPARATOR)
		next = get_next();
	return next;
};
#pragma endregion