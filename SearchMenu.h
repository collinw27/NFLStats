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
		sf::IntRect bounds;
		sf::RectangleShape rect;
		sf::Text text;
		bool selected = false;

	public:

		SearchWidget( std::string string, int index, sf::Font& font );

		// Control methods
		void draw( sf::RenderWindow* window );
		bool wasClicked( sf::Vector2i clickPos );
		void setSelected( bool selected );
	};

	WindowData* window;
    std::vector<SearchWidget*> widgets;
    Button* confirmButton;

    void addWidget( std::string text );

public:

	// See "MainMenu.h" for an explanation of the Action enum
	enum Action
	{
		NO_ACTION,
		MAIN_MENU
	};

	Action action = NO_ACTION;

	SearchMenu( WindowData* window );
    void update();
};