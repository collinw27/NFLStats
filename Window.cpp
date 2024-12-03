#include "Window.h"

Window::Window()
{
	window = new sf::RenderWindow { sf::VideoMode( 1280, 720 ), "NFL Stats" };
	font.loadFromFile( "res/Arial.ttf" );
	testButton = new Button( sf::IntRect( 20, 20, 100, 100 ), sf::Color( 0, 255, 0 ), "Click here", font );
}

bool Window::isOpen()
{
	return window->isOpen();
}

void Window::update()
{
	sf::Event event;
    while ( window->pollEvent( event ) )
    {
        if ( event.type == sf::Event::Closed )
            window->close();
    }

    window->clear( sf::Color::White );
    testButton->draw( window );
    window->display();
}