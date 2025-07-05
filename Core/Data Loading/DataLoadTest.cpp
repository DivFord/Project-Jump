#include "DataLoadTest.h"

#include <iostream>

#include "Tokeniser.h"
#include "Token.h"
#include "EntityDef.h"
#include "Other Defs/AnimDefs.h"

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
		try {
			AnimDef anim(tokeniser);
			std::cout << anim << '\n';
		}
		catch (DataLoadingException& e)
		{
			e.print();
		}
	}
};