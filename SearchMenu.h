#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"

class SearchMenu
{
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

	enum Action
	{
		NO_ACTION
	};

	Action action = NO_ACTION;

	SearchMenu( WindowData* window );
    void update();
};