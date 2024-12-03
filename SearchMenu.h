#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"

// This menu allows you to input what parameter(s) will be searched for
class SearchMenu
{
	// Allows easily rendering all the filter options
	// Also stores data relating to each filter
	class SearchWidget
	{
		sf::RectangleShape rect;
		sf::Text text;
		bool selected;

	public:

		SearchWidget( std::string string, int index, sf::Font& font );
		void draw( sf::RenderWindow* window );
	};

    std::vector<SearchWidget*> widgets;
	WindowData* window;

    void addWidget( std::string text );

public:

	// See "MainMenu.h" for an explanation of the Action enum
	enum Action
	{
		NO_ACTION
	};

	Action action = NO_ACTION;

	SearchMenu( WindowData* window );
    void update();
};