#include "MainMenu.h"

MainMenu::MainMenu( WindowData* window )
{
	// Store a reference to the program data
    this->window = window;

    // Create a button in the center of the screen
    // This will open a sub-menu if clicked
	chooseButton = new Button( sf::IntRect( 640 - 150, 360 - 40, 300, 80 ), sf::Color( 150, 150, 150 ), "Modify search", window->font );
}

void MainMenu::update()
{
	action = NO_ACTION;

	// Signal the window to change state if the button is clicked
	for ( auto click : window->clicks )
		if ( chooseButton->wasClicked( click ) )
			action = SEARCH_MENU;

	// Render the menu elements
    chooseButton->draw( window->window );
}