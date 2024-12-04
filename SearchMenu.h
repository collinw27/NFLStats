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
		sf::Text weightText;
		int weight = 0;

		// Helps with entering text
		bool typingReset = true;

	public:

		SearchWidget( std::string string, int index, sf::Font& font );

		// Control methods
		void draw( sf::RenderWindow* window );
		bool tryClick( sf::Vector2i clickPos );
		int getWeight();
		void enterText( std::vector<sf::Keyboard::Key>& keys );
	};

	WindowData* window;
    std::vector<SearchWidget*> widgets;
    SearchWidget* typingWidget;

    // Buttons
    Button* searchGamesButton;

    void addWidget( std::string text );

public:

	// The action variable reports to the window whether its state
	// needs to be updated
	// This allows the menu to change the state of the program without
	// needing to have a direct reference to the Window object
	enum Action
	{
		NO_ACTION,
		GAMES_MENU
	};

	Action action = NO_ACTION;

	SearchMenu( WindowData* window );
    void update();
    std::vector<int> getWeights();
};