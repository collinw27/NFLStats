#include "SearchMenu.h"
#include <algorithm>

// + = + = + = + = + = + = + = + = + = +
//        SEARCH WIDGET CLASS
// + = + = + = + = + = + = + = + = + = +

// Similar to the Button constructor
// Creates SFML elements for rendering the object
SearchMenu::SearchWidget::SearchWidget( std::string string, int index, sf::Font& font )
{
    bounds = sf::IntRect( 640 + 505 * ( index / 10 - 1 ), 50 + 42 * ( index % 10 ), 500, 40 );

	rect = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    rect.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    rect.setFillColor( sf::Color( 150, 150, 150 ) );
    
    text = sf::Text( string, font, 24 );
    text.setFillColor( sf::Color::Black );
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin( textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f );
    text.setPosition( sf::Vector2f( bounds.left + 250, bounds.top + 20 ) );
    
    weightText = sf::Text( "", font, 24 );
    weightText.setFillColor( sf::Color::Blue );
    weightText.setOrigin( 0, textRect.top + textRect.height / 2.f );
    weightText.setPosition( sf::Vector2f( bounds.left + 10, bounds.top + 20 ) );
}

// Drawing requires drawing both SFML shapes
void SearchMenu::SearchWidget::draw( sf::RenderWindow* window )
{
	window->draw( rect );
	window->draw( text );
    window->draw( weightText );
}

// Modifies the visuals and internal state of the widget if it was clicked
bool SearchMenu::SearchWidget::tryClick( sf::Vector2i clickPos )
{
    if ( bounds.contains( clickPos ) )
    {
        weight = ( weight == 0 ) ? 1 : 0;
        weightText.setString( "" );
        typingReset = true;
        rect.setFillColor( ( weight != 0 ) ? sf::Color( 150, 255, 150 ) : sf::Color( 150, 150, 150 ) );
        return true;
    }
    return false;
}

// Returns the weight to be used when sorting
// 0 = unselected, 1 = selected, other = advanced
int SearchMenu::SearchWidget::getWeight()
{
    return weight;
}

void SearchMenu::SearchWidget::enterText( std::vector<sf::Keyboard::Key>& keys )
{
    for ( auto key : keys )
    {   
        // Probably a better way of doing this, but this gives flexibiliy at least
        int newNum = -1;
        if ( key == sf::Keyboard::Num0 ) newNum = 0;
        else if ( key == sf::Keyboard::Num1 ) newNum = 1;
        else if ( key == sf::Keyboard::Num2 ) newNum = 2;
        else if ( key == sf::Keyboard::Num3 ) newNum = 3;
        else if ( key == sf::Keyboard::Num4 ) newNum = 4;
        else if ( key == sf::Keyboard::Num5 ) newNum = 5;
        else if ( key == sf::Keyboard::Num6 ) newNum = 6;
        else if ( key == sf::Keyboard::Num7 ) newNum = 7;
        else if ( key == sf::Keyboard::Num8 ) newNum = 8;
        else if ( key == sf::Keyboard::Num9 ) newNum = 9;
        if ( newNum != -1 )
        {
            if ( typingReset )
                weight = 0;
            weight = std::stoi( std::to_string( weight ) + std::to_string( newNum ) );
            weight = std::max( -9999, std::min( 9999, weight ) );
            typingReset = false;
        }
        if ( key == sf::Keyboard::Hyphen )
            weight *= -1;
        if ( weight == 0 || weight == 1 )
            weightText.setString( "" );
        else
            weightText.setString( std::to_string( weight ) + "x" );
    }
}

// + = + = + = + = + = + = + = + = + = +
//         SEARCH MENU CLASS
// + = + = + = + = + = + = + = + = + = +

SearchMenu::SearchMenu( WindowData* window )
{
	this->window = window;

    // Create a SFML widget for all the enterable parameters
    addWidget( "Pass attempts" );
    addWidget( "Complete passes" );
    addWidget( "Incomplete passes" );
    addWidget( "Passing yards" );
    addWidget( "Passing air yards" );
    addWidget( "Passing TDs" );
    addWidget( "Interceptions" );
    addWidget( "Targets" );
    addWidget( "Receptions" );
    addWidget( "Receiving yards" );
    addWidget( "Receiving air yards" );
    addWidget( "Yards after catch" );
    addWidget( "Reception TDs" );
    addWidget( "Rush attempts" );
    addWidget( "Rushing yards" );
    addWidget( "Rushing TDs" );
    addWidget( "Touches" );
    addWidget( "Total TDs" );
    addWidget( "Total yards" );

    // Create a button to confirm changes & exit this menu
    searchGamesButton = new Button( sf::IntRect( 640 - 150, 550 - 40, 300, 80 ), sf::Color( 100, 255, 100 ), "Search", window->font );
}

// Shorthand for creating a new widget
void SearchMenu::addWidget( std::string text )
{
    widgets.push_back( new SearchWidget( text, widgets.size(), window->font ) );
}

void SearchMenu::update()
{
    action = NO_ACTION;

    // Select only the widget that was clicked
    if ( !window->clicks.empty() )
    {
        // Deselect the current text box
        typingWidget = nullptr;

        if ( searchGamesButton->wasClicked( window->clicks[0] ) )
            action = GAMES_MENU;
        for ( auto widget : widgets )
        {
            if ( widget->tryClick( window->clicks[0] ) )
                typingWidget = widget;
        }
    }

    // Type in the widget
    if ( !window->keys.empty() && typingWidget != nullptr )
        typingWidget->enterText( window->keys );

    // Render the menu elements
    for ( auto widget : widgets )
        widget->draw( window->window );
    searchGamesButton->draw( window->window );
}

// Return the weights for each parameter based on what the user selected
// This is passed into the database and used in the sorting functions
std::vector<int> SearchMenu::getWeights()
{
    std::vector<int> output;
    for ( auto widget : widgets )
        output.push_back( widget->getWeight() );
    return output;
}