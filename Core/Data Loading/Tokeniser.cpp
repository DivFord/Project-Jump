#include "Tokeniser.h"

#include <iostream>

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
	while (fileStream.good())
	{
		fileStream.getline(buffer, bufferSize);
		process_line(buffer);
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

void Tokeniser::process_line(const char* line)
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
			output.push(Token(Token::Type::BRACKET, std::string(line, 1)));
			line++;
		}

		else if (current == ',')
		{
			output.push(Token(Token::Type::SEPARATOR, std::string(line, 1)));
			line++;
		}

		else if (current == '=' || current == '+' || current == '*' || current == '/')
		{
			output.push(Token(Token::Type::OPERATOR, std::string(line, 1)));
			line++;
		}

		else if (current == '-' || char_is_number(current))
		{
			const char* next = line + 1;
			while (char_is_number(*next))
				next++;
			if (next - line == 1 && current == '-') {
				output.push(Token(Token::Type::OPERATOR, std::string(line, 1)));
				line++;
			}
			else {
				output.push(Token(Token::Type::NUMBER, std::string(line, next - line)));
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
				output.push(Token(Token::Type::STRING, std::string(line + 1, next - line - 1)));
				line = next + 1;
			}
			else
			{
				output.push(Token(Token::Type::STRING, std::string(line + 1, next - line)));
				line = next;
			}
		}

		else {
			const char* next = line + 1;
			while (!char_breaks_string(*next, false))
				next++;
			std::string val(line, next - line);
			if (classNames.count(val) > 0)
				output.push(Token(Token::Type::CLASS_NAME, val));
			else
				output.push(Token(Token::Type::VAR_NAM, val));
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
#pragma endregion