#include "ResultsMenu.h"
#include <algorithm>

// + = + = + = + = + = + = + = + = + = +
//        RESULTS WIDGET CLASS
// + = + = + = + = + = + = + = + = + = +

ResultsMenu::ResultWidget::ResultWidget( int index, sf::Font& font )
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

// Update the visuals based on which information was passed into it
void ResultsMenu::ResultWidget::update( GameStats* game, Player* player, int newIndex )
{
    // Make this invisible if the game is invalid (i.e. nullptr)
    this->game = game;
    this->player = player;
    rect.setFillColor( ( game || player ) ? sf::Color( 150, 150, 150 ) : sf::Color::White );

    // If the game is valid, show the player, rank, season, and week
    std::string string = "";
    if ( game )
    {
        string += "#" + std::to_string( newIndex + 1 ) + " - " + game->player->name;
        string += " [S=" + std::to_string( game->score ) + "]\n";
        string += "Season " + std::to_string( game->season ) + ", Week " + std::to_string( game->week );
    }
    else if ( player )
    {
        string += "#" + std::to_string( newIndex + 1 ) + " - " + player->name;
        string += " [S=" + std::to_string( player->score ) + "]\n";
    }
    text.setString( string );
}

void ResultsMenu::ResultWidget::draw( sf::RenderWindow* window )
{
    window->draw( rect );
    window->draw( text );
}

bool ResultsMenu::ResultWidget::wasClicked( sf::Vector2i clickPos )
{
    return bounds.contains( clickPos.x, clickPos.y );
}

// + = + = + = + = + = + = + = + = + = +
//         RESULTS MENU CLASS
// + = + = + = + = + = + = + = + = + = +

// To display players, pass in an empty vector for games
// To display games, pass in an empty vector for players
ResultsMenu::ResultsMenu( WindowData* window, const std::vector<GameStats*>& games, const std::vector<Player*>& players )
{
    this->window = window;
    this->games = games;
    this->players = players;

    // Create widgets to hold/display the games on screen
    for ( int i = 0; i < 4; ++i )
        widgets.push_back( new ResultWidget( i, window->font ) );

    // Create a button to exit this menu
    backButton = new Button( sf::IntRect( 20, 20, 150, 80 ), sf::Color( 220, 220, 220 ), "Back", window->font );

    // Create the text to display result data
    currentResult = sf::Text( "", window->font, 22 );
    currentResult.setFillColor( sf::Color::Black );
    currentResult.setPosition( sf::Vector2f( 640 - 450, 360 - 320 ) );

    // Create a background for the text
    currentResultBG = sf::RectangleShape( sf::Vector2f( 900, 640 ) );
    currentResultBG.setPosition( sf::Vector2f( 640 - 450, 360 - 320 ) );
    currentResultBG.setFillColor( sf::Color( 210, 210, 255 ) );
}

void ResultsMenu::update()
{
    action = NO_ACTION;

    // Scroll up and down if necessary
    int oldScroll = scroll;
    if ( !showingResult )
    {
        scroll -= window->mouseScroll;
        if ( !games.empty() )
            scroll = std::max( 0, std::min( scroll, (int)( games.size() - 1 ) ) );
        else
            scroll = std::max( 0, std::min( scroll, (int)( players.size() - 1 ) ) );
    }

    if ( !window->clicks.empty() )
    {
        if ( !showingResult )
        {
            // Close if necessary
            if ( backButton->wasClicked( window->clicks[0] ) )
                action = BACK;

            // Click on invididual games
            // This makes more information about them pop up
            if ( !games.empty() )
            {
                for ( int i = 0; i < 4; ++i )
                    if ( widgets[i]->wasClicked( window->clicks[0] ) && i + scroll < games.size() )
                        setCurrentGame( games[ i + scroll ] );
            }
            else
            {
                for ( int i = 0; i < 4; ++i )
                    if ( widgets[i]->wasClicked( window->clicks[0] ) && i + scroll < players.size() )
                        setCurrentPlayer( players[ i + scroll ] );
            }
        }
        else
            if ( !sf::IntRect( 640 - 450, 360 - 320, 900, 640 ).contains( window->clicks[0] ) )
            {
                setCurrentGame( nullptr );
            }
    }

    // Modify the UI upon being created or scrolled
    if ( oldScroll != scroll )
    {
        for ( int i = 0; i < 4; ++i )
            widgets[i]->update(
                ( !games.empty() && i + scroll < games.size() ) ? games[ i + scroll ] : nullptr,
                ( games.empty() && i + scroll < players.size() ) ? players[ i + scroll ] : nullptr,
                i + scroll
            );
    }

    // Draw the visuals to the screen
    backButton->draw( window->window );
    for ( auto widget : widgets )
        widget->draw( window->window );
    if ( showingResult )
    {
        window->window->draw( currentResultBG );
        window->window->draw( currentResult );
    }
}

// Displays stats about the selected game in a popup
// Pass in nullptr to close popup
void ResultsMenu::setCurrentGame( GameStats* game )
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
    currentResult.setString( string );
    showingResult = ( game != nullptr );
}

// Displays stats about the selected player in a popup
// Pass in nullptr to close popup
void ResultsMenu::setCurrentPlayer( Player* player )
{
    std::string string;
    if ( player != nullptr )
    {
        string += player->name + "\n";
        string += "Teams: ";
        for ( auto team : player->teams )
            string += team + " ";
        string += "\n";
        string += "Height: " + std::to_string( player->height ) + " in.\n";
        string += "Weight: " + std::to_string( player->weight ) + " lbs.\n";
        string += "Avg. Pass attempts: " + std::to_string( player->passAttempts / player->numDataPoints ) + "\n";
        string += "Avg. Complete passes: " + std::to_string( player->completedPasses / player->numDataPoints ) + "\n";
        string += "Avg. Incomplete passes: " + std::to_string( player->incompletePasses / player->numDataPoints ) + "\n";
        string += "Avg. Passing yards: " + std::to_string( player->passingYards / player->numDataPoints ) + "\n";
        string += "Avg. Passing air yards: " + std::to_string( player->passingAirYards / player->numDataPoints ) + "\n";
        string += "Avg. Passing TDs: " + std::to_string( player->passTD / player->numDataPoints ) + "\n";
        string += "Avg. Interceptions: " + std::to_string( player->interceptions / player->numDataPoints ) + "\n";
        string += "Avg. Targets: " + std::to_string( player->targets / player->numDataPoints ) + "\n";
        string += "Avg. Receptions: " + std::to_string( player->receptions / player->numDataPoints ) + "\n";
        string += "Avg. Receiving yards: " + std::to_string( player->receivingYards / player->numDataPoints ) + "\n";
        string += "Avg. Receiving air yards: " + std::to_string( player->receivingAirYards / player->numDataPoints ) + "\n";
        string += "Avg. Yards after catch: " + std::to_string( player->yardsAfterCatch / player->numDataPoints ) + "\n";
        string += "Avg. Reception TDs: " + std::to_string( player->receptionTD / player->numDataPoints ) + "\n";
        string += "Avg. Rush attempts: " + std::to_string( player->rushAttempts / player->numDataPoints ) + "\n";
        string += "Avg. Rushing yards: " + std::to_string( player->rushingYards / player->numDataPoints ) + "\n";
        string += "Avg. Rushing TDs: " + std::to_string( player->rushingTD / player->numDataPoints ) + "\n";
        string += "Avg. Touches: " + std::to_string( player->touches / player->numDataPoints ) + "\n";
        string += "Avg. Total TDs: " + std::to_string( player->totalTD / player->numDataPoints ) + "\n";
        string += "Avg. Total yards: " + std::to_string( player->totalYards / player->numDataPoints ) + "\n";
    }
    currentResult.setString( string );
    showingResult = ( player != nullptr );
}