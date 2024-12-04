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
		sf::IntRect bounds;
		sf::RectangleShape rect;
		sf::Text text;
		GameStats* game;

	public:

		GameWidget( int index, sf::Font& font );

		// Control methods
		void update( GameStats* game, int newIndex );
		void draw( sf::RenderWindow* window );
		bool wasClicked( sf::Vector2i clickPos );
	};

	// State
	WindowData* window;
    std::vector<GameStats*> games;
    bool showingGame = false;

    // Display elements
    std::vector<GameWidget*> widgets;
    Button* backButton;
    sf::Text currentGame;
    sf::RectangleShape currentGameBG;

    int scroll = -1;

public:

	// See "SearchMenu.h" for an explanation of Action enums
	enum Action
	{
		NO_ACTION,
		BACK
	};

	Action action = NO_ACTION;

	GamesMenu( WindowData* window, std::vector<GameStats*>& games );

	// Control methods
    void update();
    void setCurrentGame( GameStats* game );
};