#include "Window.h"
#include "MainMenu.h"
#include "SearchMenu.h"

Button::Button( sf::IntRect bounds, sf::Color color, std::string string, sf::Font& font ) : bounds{ bounds }
{
	shape = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    shape.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    shape.setFillColor( color );
    
    text = sf::Text( string, font );
    text.setFillColor( sf::Color::Black );
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin( textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f );
    text.setPosition( sf::Vector2f( bounds.left, bounds.top ) + shape.getSize() / 2.f );
}

bool Button::wasClicked( sf::Vector2i clickPos )
{
	return bounds.contains( clickPos );
}

void Button::draw( sf::RenderWindow* window )
{
	window->draw( shape );
	window->draw( text );
}

Window::Window()
{
	windowData = new WindowData { new sf::RenderWindow( sf::VideoMode( 1280, 720 ), "NFL Stats" ) };
	windowData->font.loadFromFile( "res/Arial.ttf" );

	mainMenu = new MainMenu( windowData );
	searchMenu = new SearchMenu( windowData );
}

bool Window::isOpen()
{
	return windowData->window->isOpen();
}

void Window::update()
{
	windowData->clicks.clear();
	windowData->keys.clear();

	sf::Event event;
    while ( windowData->window->pollEvent( event ) )
    {
        if ( event.type == sf::Event::Closed )
            windowData->window->close();

        else if ( event.type == sf::Event::MouseButtonPressed )
        {
		    if ( event.mouseButton.button == sf::Mouse::Left )
		    	windowData->clicks.push_back( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ) );
        }

        else if ( event.type == sf::Event::KeyPressed )
		    windowData->keys.push_back( event.key.code );
    }

    windowData->window->clear( sf::Color::White );

    if ( state == MAIN_MENU )
    {
	    mainMenu->update();
	    if ( mainMenu->action == MainMenu::SEARCH_MENU )
	    	state = SEARCH_MENU;
    }
    else if ( state == SEARCH_MENU )
    {
	    searchMenu->update();
    }

    windowData->window->display();
}