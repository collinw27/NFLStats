#include "SearchMenu.h"

// + = + = + = + = + = + = + = + = + = +
//        SEARCH WIDGET CLASS
// + = + = + = + = + = + = + = + = + = +

// Similar to the Button constructor
// Creates SFML elements for rendering the object
SearchMenu::SearchWidget::SearchWidget( std::string string, int index, sf::Font& font )
{
    bounds = sf::IntRect( 100, 100 + 40 * index, 300, 40 );

	rect = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    rect.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    rect.setFillColor( sf::Color( 150, 150, 150 ) );
    
    text = sf::Text( string, font, 24 );
    text.setFillColor( sf::Color::Black );
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin( textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f );
    text.setPosition( sf::Vector2f( bounds.left + 150, bounds.top + 20 ) );
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

    // Create a button to confirm changes & exit this menu
    confirmButton = new Button( sf::IntRect( 640 - 150, 500 - 40, 300, 80 ), sf::Color( 100, 255, 100 ), "Confirm", window->font );
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
        for ( auto widget : widgets )
            widget->setSelected( widget->wasClicked( window->clicks[0] ) );

    // Signal the window to change state if the button is clicked
    for ( auto click : window->clicks )
        if ( confirmButton->wasClicked( click ) )
            action = MAIN_MENU;

    // Render the menu elements
    for ( auto widget : widgets )
        widget->draw( window->window );
    confirmButton->draw( window->window );
}