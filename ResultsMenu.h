#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"

// Displays and allows moving between the returned games/players
// Since it can show results for multiple data types (GameStats/Player),
// some duplicate variables are necessary so they have the correct type
// However, this is a better alternative to copy and pasting 2 different
// classes for GamesMenu/PlayersMenu
class ResultsMenu
{
	// Allows easily rendering all the games
	class ResultWidget
	{
		sf::IntRect bounds;
		sf::RectangleShape rect;
		sf::Text text;

		// One or both should be nullptr
		GameStats* game;
		Player* player;

	public:

		ResultWidget( int index, sf::Font& font );

		// Control methods
		void update( GameStats* game, Player* player, int newIndex );
		void draw( sf::RenderWindow* window );
		bool wasClicked( sf::Vector2i clickPos );
	};

	// State
	// Different variables for games/players when necessary
	WindowData* window;
    std::vector<GameStats*> games;
    std::vector<Player*> players;
    bool showingResult = false;

    // Display elements
    std::vector<ResultWidget*> widgets;
    Button* backButton;
    sf::Text currentResult;
    sf::RectangleShape currentResultBG;

    int scroll = -1;

public:

	// See "SearchMenu.h" for an explanation of Action enums
	enum Action
	{
		NO_ACTION,
		BACK
	};

	Action action = NO_ACTION;

	ResultsMenu( WindowData* window, const std::vector<GameStats*>& games, const std::vector<Player*>& players );
	~ResultsMenu();

	// Control methods
    void update();
    void setCurrentGame( GameStats* game );
    void setCurrentPlayer( Player* player );
};