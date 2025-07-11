// Project Jump.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

#include <pandaFramework.h>
#include <pandaSystem.h>
#include <windowProperties.h>

#include "Core/Game.h"
#include "Core/Config.h"

#include "Core/Data Loading/DataLoadTest.h"

int main(int argc, char* argv[])
{
    //DataLoadTest dataTest(true);

    InitNewTypes();

    //Open a new window framework.
    PandaFramework framework;
    framework.open_framework(argc, argv);

    //Set the window title and size, then open the window.
    WindowProperties windowProps;
    windowProps.set_size(1000, 750);
    //windowProps.set_title(std::string());//BUG: WINDOW TITLE IS GETTING SURROUNDED WITH JUNK CHARACTERS.
    //std::cout << windowProps.get_title() << '\n'; //BUG: CAUSES MEMORY VIOLATION.
    WindowFramework* window = framework.open_window(windowProps, 0);

    //Create the game!
    Game game(framework, *window);

    //Run the main loop.
    framework.main_loop();

    //Close and exit.
    framework.close_framework();

    return 0;
};
