#include "Window.h"
#include "SearchMenu.h"
#include "GamesMenu.h"

// + = + = + = + = + = + = + = + = + = +
//            BUTTON CLASS
// + = + = + = + = + = + = + = + = + = +

// Store the bounds rect for easy click detection
Button::Button( sf::IntRect bounds, sf::Color color, std::string string, sf::Font& font, int size ) : bounds{ bounds }
{
	// Buttons are made of 2 SFML shapes: sf::Text and sf::Rectangle
	// Start by creating the rectangle using the given parameters
	shape = sf::RectangleShape( sf::Vector2f( bounds.width, bounds.height ) );
    shape.setPosition( sf::Vector2f( bounds.left, bounds.top ) );
    shape.setFillColor( color );
    
    // Then, create the text object
    // The line with setOrigin() centers it within the rectangle
    text = sf::Text( string, font, size );
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

// Switches the parameters of the SFML object
void Button::change( std::string string, sf::Color color )
{
    text.setString( string );
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin( textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f );
    text.setPosition( sf::Vector2f( bounds.left, bounds.top ) + shape.getSize() / 2.f );
    shape.setFillColor( color );
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
	searchMenu = new SearchMenu( windowData );

    // Create the database
    database = new StatsDatabase( "weekly_player_data_full.csv" );
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
    windowData->mouseScroll = 0;

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

        else if ( event.type == sf::Event::MouseWheelScrolled )
            windowData->mouseScroll = event.mouseWheelScroll.delta;
    }

    // Start drawing the next frame
    windowData->window->clear( sf::Color::White );

    // Search menu state:
    // Switch to games menu when "Search Games" is pressed
    if ( state == SEARCH_MENU )
    {
        searchMenu->update();

        // Search for the selected games
        if ( searchMenu->action == SearchMenu::GAMES_MENU )
        {
            std::vector<int> weights = searchMenu->getWeights();
            database->buildHeap( weights );
            std::vector<GameStats*> topGames = database->extractGames( 50 );
            delete gamesMenu;
            gamesMenu = new GamesMenu( windowData, topGames );
            state = GAMES_MENU;
        }
    }

    // Games menu state:
    // Go back to search when "Back" is pressed
    else if ( state == GAMES_MENU )
    {
        gamesMenu->update();
        if ( gamesMenu->action == GamesMenu::BACK )
            state = SEARCH_MENU;
    }

    // Display the buffered frame
    windowData->window->display();
}