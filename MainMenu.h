#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Window.h"

class MainMenu
{
	WindowData* window;
	Button* chooseButton;

public:

	enum Action
	{
		NO_ACTION,
		SEARCH_MENU
	};

	Action action = NO_ACTION;

	MainMenu( WindowData* window );
	void update();
};