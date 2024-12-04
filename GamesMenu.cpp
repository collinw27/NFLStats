#include "GamesMenu.h"
#include <algorithm>

// + = + = + = + = + = + = + = + = + = +
//          GAME WIDGET CLASS
// + = + = + = + = + = + = + = + = + = +

GamesMenu::GameWidget::GameWidget( int index, sf::Font& font )
{
    // Background: A SFML rect
    rect = sf::RectangleShape( sf::Vector2f( 800, 120 ) );
    rect.setPosition( sf::Vector2f( ( 1280 - 800 ) / 2, 60 + 150 * index ) );
    rect.setFillColor( sf::Color( 200, 200, 200 ) );
    color = ( index == 0 ) ? sf::Color( 150, 150, 255 ) : sf::Color( 150, 150, 150 );
    
    // Then, create the text object
    // Centered in the top-left
    text = sf::Text( "", font );
    text.setFillColor( sf::Color::Black );
    text.setPosition( rect.getPosition() + sf::Vector2f( 10, 10 ) );
}

// Update the visuals based on which game information was passed into it
void GamesMenu::GameWidget::update( GameStats* game, int newIndex )
{
    this->game = game;
    rect.setFillColor( ( game ) ? color : sf::Color::White );
    std::string string = "";

    if ( game )
    {
        string += "#" + std::to_string( newIndex + 1 ) + " - " + game->player->name + "\n";
        string += "Season " + std::to_string( game->season ) + ", Week " + std::to_string( game->week );
    }
    text.setString( string );
}

void GamesMenu::GameWidget::draw( sf::RenderWindow* window )
{
    window->draw( rect );
    window->draw( text );
}

// + = + = + = + = + = + = + = + = + = +
//          GAMES MENU CLASS
// + = + = + = + = + = + = + = + = + = +

GamesMenu::GamesMenu( WindowData* window, std::vector<GameStats*>& games )
{
    this->window = window;
    this->games = games;

    // Create widgets to hold/display the games on screen
    for ( int i = 0; i < 4; ++i )
        widgets.push_back( new GameWidget( i, window->font ) );
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
        for ( int i = 0; i < 4; ++i )
            widgets[i]->update( ( i + scroll < games.size() ) ? games[ i + scroll ] : nullptr, i + scroll );
    }

    for ( auto widget : widgets )
        widget->draw( window->window );
}