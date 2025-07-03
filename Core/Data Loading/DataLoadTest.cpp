#include "DataLoadTest.h"

#include <iostream>

#include "Tokeniser.h"
#include "Token.h"
#include "Component Defs/Physics/FixedShapePhysicsDef.h"

DataLoadTest::DataLoadTest(bool printTokens)
{
	Tokeniser tokeniser;
	tokeniser.process_file("TestData.txt");

	if (printTokens)
	{
		Token current = tokeniser.get_current();
		while (!current.unset())
		{
			std::cout << current.print() << '\n';
			current = tokeniser.get_next();
		}
	}

	else
	{
		tokeniser.advance_until(Token::Type::NUMBER);
		std::cout << tokeniser.get_current().print() << '\n';
		std::cout << VariableLoader::load_number(tokeniser);
	}
};