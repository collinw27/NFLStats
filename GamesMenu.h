#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"

// Displays and allows moving between the returned games
class GamesMenu
{
	WindowData* window;
    std::vector<GameStats*> games;
    Button* backButton;

    int scroll = -1;

public:

	GamesMenu( WindowData* window, std::vector<GameStats*>& games );
    void update();
};