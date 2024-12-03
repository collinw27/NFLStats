#include <iostream>
#include <fstream>
#include "StatsDatabase.h"
#include "Window.h"

int main()
{
    // The Window class is responsible for managing all updates within the program
    // It contains the database that is used for searching through NFL players/games
    // Keep updating the window until the user closes it
    Window window;
    while ( window.isOpen() )
    	window.update();

    return 0;
}
