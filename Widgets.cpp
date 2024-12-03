#include "Widgets.h"

Button::Button( sf::IntRect bounds, sf::Color color, std::string string, sf::Font& font ) : bounds{ bounds }
{
	shape = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    shape.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    shape.setFillColor( color );
    
    text = sf::Text( string, font );
    text.setColor( sf::Color::Black );
    text.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
}

void Button::draw( sf::RenderWindow* window )
{
	window->draw( shape );
	window->draw( text );
}