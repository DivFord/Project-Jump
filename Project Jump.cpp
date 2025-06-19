// Project Jump.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

#include <pandaFramework.h>
#include <pandaSystem.h>
#include <windowProperties.h>

struct Test
{
    std::string testStr;

    void test_method(const std::string& test)
    {
        testStr = test;
    }
};

int main(int argc, char* argv[])
{
    std::string testStr = "Hello";
    Test test;
    test.test_method(testStr);
    std::cout << test.testStr << '\n';

    //Open a new window framework.
    PandaFramework framework;
    framework.open_framework(argc, argv);

    //Set the window title and size, then open the window.
    WindowProperties windowProps;
    windowProps.set_size(1000, 750);
    //windowProps.set_title(std::string());//BUG: WINDOW TITLE IS GETTING SURROUNDED WITH JUNK CHARACTERS.
    //std::cout << windowProps.get_title() << '\n'; //BUG: CAUSES MEMORY VIOLATION.
    WindowFramework* window = framework.open_window(windowProps, 0);

    //Run the main loop.
    framework.main_loop();

    //Close and exit.
    framework.close_framework();
    return 0;
}
