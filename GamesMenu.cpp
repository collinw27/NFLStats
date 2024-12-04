#include "GamesMenu.h"
#include <algorithm>

GamesMenu::GamesMenu( WindowData* window, std::vector<GameStats*>& games )
{
    this->window = window;
    this->games = games;
}

void GamesMenu::update()
{
    // Scroll up and down if necessary
    int oldScroll = scroll;
    for ( auto key : window->keys )
    {
        if ( key == sf::Keyboard::Down )
            ++scroll;
        else if ( key == sf::Keyboard::Up )
            --scroll;
    }
    scroll = std::max( 0, std::min( scroll, (int)( games.size() - 1 ) ) );

    // Modify the UI upon being created or scrolled
    if ( oldScroll != scroll )
    {
        
    }
}