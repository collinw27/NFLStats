#include "SearchMenu.h"

SearchMenu::SearchWidget::SearchWidget( std::string string, int index, sf::Font& font )
{
	rect = sf::RectangleShape( sf::Vector2f( 300, 40 ) );
    rect.setPosition( sf::Vector2f( 100, 100 + 40 * index ) );
    rect.setFillColor( sf::Color( 150, 150, 150 ) );
    
    text = sf::Text( string, font );
    text.setFillColor( sf::Color::Black );
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin( textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f );
    text.setPosition( sf::Vector2f( 250, 120 + 40 * index ) );
}

void SearchMenu::SearchWidget::draw( sf::RenderWindow* window )
{
	window->draw( rect );
	window->draw( text );
}

SearchMenu::SearchMenu( WindowData* window )
{
	this->window = window;

    addWidget( "Pass attempts" );
    addWidget( "Complete passes" );
    addWidget( "Incomplete passes" );
}

void SearchMenu::update()
{
    for ( auto widget : widgets )
        widget->draw( window->window );
}

void SearchMenu::addWidget( std::string text )
{
    widgets.push_back( new SearchWidget( text, widgets.size(), window->font ) );
}