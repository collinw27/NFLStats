#include <iostream>
#include <fstream>
#include "StatsDatabase.h"
#include "Window.h"

int main()
{
    StatsDatabase database( "weekly_player_data_full.csv" );
    Window window;
    while ( window.isOpen() )
    	window.update();

    return 0;
}
