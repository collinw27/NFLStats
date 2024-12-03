#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Window.h"

// The MainMenu is where most of the options are displayed
class MainMenu
{
	WindowData* window;
	Button* chooseButton;

public:

	// The action variable reports to the window whether its state
	// needs to be updated
	// This allows the menu to change the state of the program without
	// needing to have a direct reference to the Window object
	enum Action
	{
		NO_ACTION,
		SEARCH_MENU
	};
	Action action = NO_ACTION;

	MainMenu( WindowData* window );
	void update();
};