#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"

// Displays and allows moving between the returned games
class GamesMenu
{
	// Allows easily rendering all the games
	class GameWidget
	{
		sf::RectangleShape rect;
		sf::Text text;
		GameStats* game;
		sf::Color color;

	public:

		GameWidget( int index, sf::Font& font );

		// Control methods
		void update( GameStats* game, int newIndex );
		void draw( sf::RenderWindow* window );
	};

	WindowData* window;
    std::vector<GameStats*> games;
    std::vector<GameWidget*> widgets;
    Button* backButton;

    int scroll = -1;

public:

	GamesMenu( WindowData* window, std::vector<GameStats*>& games );
    void update();
};