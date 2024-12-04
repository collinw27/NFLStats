#include "GamesMenu.h"
#include <algorithm>

// + = + = + = + = + = + = + = + = + = +
//          GAME WIDGET CLASS
// + = + = + = + = + = + = + = + = + = +

GamesMenu::GameWidget::GameWidget( int index, sf::Font& font )
{
    bounds = sf::IntRect( ( 1280 - 800 ) / 2, 60 + 150 * index, 800, 120 );

    // Background: A SFML rect
    rect = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    rect.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    rect.setFillColor( sf::Color( 200, 200, 200 ) );
    
    // Then, create the text object
    // Centered in the top-left
    text = sf::Text( "", font );
    text.setFillColor( sf::Color::Black );
    text.setPosition( rect.getPosition() + sf::Vector2f( 10, 10 ) );
}

// Update the visuals based on which game information was passed into it
void GamesMenu::GameWidget::update( GameStats* game, int newIndex )
{
    // Make this invisible if the game is invalid (i.e. nullptr)
    this->game = game;
    rect.setFillColor( ( game ) ? sf::Color( 150, 150, 150 ) : sf::Color::White );

    // If the game is valid, show the player, rank, season, and week
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

bool GamesMenu::GameWidget::wasClicked( sf::Vector2i clickPos )
{
    return bounds.contains( clickPos.x, clickPos.y );
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

    // Create a button to exit this menu
    backButton = new Button( sf::IntRect( 20, 20, 150, 80 ), sf::Color( 220, 220, 220 ), "Back", window->font );

    // Create the text to display game data
    currentGame = sf::Text( "", window->font, 22 );
    currentGame.setFillColor( sf::Color::Black );
    currentGame.setPosition( sf::Vector2f( 640 - 450, 360 - 320 ) );

    // Create a background for the text
    currentGameBG = sf::RectangleShape( sf::Vector2f( 900, 640 ) );
    currentGameBG.setPosition( sf::Vector2f( 640 - 450, 360 - 320 ) );
    currentGameBG.setFillColor( sf::Color( 210, 210, 255 ) );
}

void GamesMenu::update()
{
    action = NO_ACTION;

    // Scroll up and down if necessary
    int oldScroll = scroll;
    if ( !showingGame )
    {
        scroll -= window->mouseScroll;
        scroll = std::max( 0, std::min( scroll, (int)( games.size() - 1 ) ) );
    }

    if ( !window->clicks.empty() )
    {
        if ( !showingGame )
        {
            // Close if necessary
            if ( backButton->wasClicked( window->clicks[0] ) )
                action = BACK;

            // Click on invididual games
            // This makes more information about them pop up
            for ( int i = 0; i < 4; ++i )
                if ( widgets[i]->wasClicked( window->clicks[0] ) && i + scroll < games.size() )
                    setCurrentGame( games[ i + scroll ] );
        }
        else
            if ( !sf::IntRect( 640 - 450, 360 - 320, 900, 640 ).contains( window->clicks[0] ) )
                setCurrentGame( nullptr );
    }

    // Modify the UI upon being created or scrolled
    if ( oldScroll != scroll )
    {
        for ( int i = 0; i < 4; ++i )
            widgets[i]->update( ( i + scroll < games.size() ) ? games[ i + scroll ] : nullptr, i + scroll );
    }

    // Draw the visuals to the screen
    backButton->draw( window->window );
    for ( auto widget : widgets )
        widget->draw( window->window );
    if ( showingGame )
    {
        window->window->draw( currentGameBG );
        window->window->draw( currentGame );
    }
}

// Displays stats about the selected game in a popup
// Pass in nullptr to close popup
void GamesMenu::setCurrentGame( GameStats* game )
{
    std::string string;
    if ( game != nullptr )
    {
        string += game->player->name + ";  Season " + std::to_string( game->season ) + ", Week " + std::to_string( game->week ) + "\n";
        string += "Position: " + game->position + "\n";
        string += "Team: " + game->team + "\n";
        string += "Opponent: " + game->opponent + "\n";
        string += "Pass attempts: " + std::to_string( game->passAttempts ) + "\n";
        string += "Complete passes: " + std::to_string( game->completedPasses ) + "\n";
        string += "Incomplete passes: " + std::to_string( game->incompletePasses ) + "\n";
        string += "Passing yards: " + std::to_string( game->passingYards ) + "\n";
        string += "Passing air yards: " + std::to_string( game->passingAirYards ) + "\n";
        string += "Passing TDs: " + std::to_string( game->passTD ) + "\n";
        string += "Interceptions: " + std::to_string( game->interceptions ) + "\n";
        string += "Targets: " + std::to_string( game->targets ) + "\n";
        string += "Receptions: " + std::to_string( game->receptions ) + "\n";
        string += "Receiving yards: " + std::to_string( game->receivingYards ) + "\n";
        string += "Receiving air yards: " + std::to_string( game->receivingAirYards ) + "\n";
        string += "Yards after catch: " + std::to_string( game->yardsAfterCatch ) + "\n";
        string += "Reception TDs: " + std::to_string( game->receptionTD ) + "\n";
        string += "Rush attempts: " + std::to_string( game->rushAttempts ) + "\n";
        string += "Rushing yards: " + std::to_string( game->rushingYards ) + "\n";
        string += "Rushing TDs: " + std::to_string( game->rushingTD ) + "\n";
        string += "Touches: " + std::to_string( game->touches ) + "\n";
        string += "Total TDs: " + std::to_string( game->totalTD ) + "\n";
        string += "Total yards: " + std::to_string( game->totalYards ) + "\n";
    }
    currentGame.setString( string );
    showingGame = ( game != nullptr );
}