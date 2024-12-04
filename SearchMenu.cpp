#include "SearchMenu.h"

// + = + = + = + = + = + = + = + = + = +
//        SEARCH WIDGET CLASS
// + = + = + = + = + = + = + = + = + = +

// Similar to the Button constructor
// Creates SFML elements for rendering the object
SearchMenu::SearchWidget::SearchWidget( std::string string, int index, sf::Font& font )
{
    bounds = sf::IntRect( 640 + 405 * ( index / 10 - 1 ), 100 + 40 * ( index % 10 ), 400, 40 );

	rect = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    rect.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    rect.setFillColor( sf::Color( 150, 150, 150 ) );
    
    text = sf::Text( string, font, 24 );
    text.setFillColor( sf::Color::Black );
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin( textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f );
    text.setPosition( sf::Vector2f( bounds.left + 200, bounds.top + 20 ) );
}

// Drawing requires drawing both SFML shapes
void SearchMenu::SearchWidget::draw( sf::RenderWindow* window )
{
	window->draw( rect );
	window->draw( text );
}

// The widget was clicked if the bounds rect contains the mouse position
bool SearchMenu::SearchWidget::wasClicked( sf::Vector2i clickPos )
{
    return bounds.contains( clickPos );
}

// Modifies the visuals and internal state of the widget
void SearchMenu::SearchWidget::setSelected( bool selected )
{
    this->selected = selected;
    rect.setFillColor( selected ? sf::Color( 150, 255, 150 ) : sf::Color( 150, 150, 150 ) );
}

// Returns the weight to be used when sorting
// 0 = unselected, 1 = selected, other = advanced
int SearchMenu::SearchWidget::getWeight()
{
    return ( selected ) ? 1 : 0;
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
    searchButton = new Button( sf::IntRect( 640 - 150, 550 - 40, 300, 80 ), sf::Color( 100, 255, 100 ), "Search", window->font );
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
        if ( searchButton->wasClicked( window->clicks[0] ) )
            action = GAMES_MENU;
        else
        {
            for ( auto widget : widgets )
                widget->setSelected( widget->wasClicked( window->clicks[0] ) );
        }

    // Render the menu elements
    for ( auto widget : widgets )
        widget->draw( window->window );
    searchButton->draw( window->window );
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