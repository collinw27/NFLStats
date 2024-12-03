#include "Window.h"
#include "MainMenu.h"
#include "SearchMenu.h"

// + = + = + = + = + = + = + = + = + = +
//            BUTTON CLASS
// + = + = + = + = + = + = + = + = + = +

// Store the bounds rect for easy click detection
Button::Button( sf::IntRect bounds, sf::Color color, std::string string, sf::Font& font ) : bounds{ bounds }
{
	// Buttons are made of 2 SFML shapes: sf::Text and sf::Rectangle
	// Start by creating the rectangle using the given parameters
	shape = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    shape.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    shape.setFillColor( color );
    
    // Then, create the text object
    // The line with setOrigin() centers it within the rectangle
    text = sf::Text( string, font );
    text.setFillColor( sf::Color::Black );
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin( textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f );
    text.setPosition( sf::Vector2f( bounds.left, bounds.top ) + shape.getSize() / 2.f );
}

// The button was clicked if the bounds rect contains the mouse position
bool Button::wasClicked( sf::Vector2i clickPos )
{
	return bounds.contains( clickPos );
}

// Drawing requires drawing both SFML shapes
void Button::draw( sf::RenderWindow* window )
{
	window->draw( shape );
	window->draw( text );
}

// + = + = + = + = + = + = + = + = + = +
//            WINDOW CLASS
// + = + = + = + = + = + = + = + = + = +

Window::Window()
{
	// Create the WindowData object, which is accessible to all menus
	windowData = new WindowData { new sf::RenderWindow( sf::VideoMode( 1280, 720 ), "NFL Stats" ) };
	windowData->font.loadFromFile( "res/Arial.ttf" );

	// Create the menus, even the ones that aren't open yet
	mainMenu = new MainMenu( windowData );
	searchMenu = new SearchMenu( windowData );
}

// Used by the main loop to decide when to end the program
bool Window::isOpen()
{
	return windowData->window->isOpen();
}

// Called every frame
void Window::update()
{
	// Erase the events from the previous frame
	windowData->clicks.clear();
	windowData->keys.clear();

	// Store clicks/keys to be checked by the menu
	// Also close the window if necessary
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

    // Start drawing the next frame
    windowData->window->clear( sf::Color::White );

    // Main menu state:
    // Transition to search menu if "Modify Search" is clicked
    if ( state == MAIN_MENU )
    {
	    mainMenu->update();
	    if ( mainMenu->action == MainMenu::SEARCH_MENU )
	    	state = SEARCH_MENU;
    }

    // Search menu state:
    // ...
    else if ( state == SEARCH_MENU )
    {
	    searchMenu->update();
    }

    // Display the buffered frame
    windowData->window->display();
}