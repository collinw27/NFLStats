#include "MainMenu.h"

MainMenu::MainMenu( WindowData* window )
{
    this->window = window;
	chooseButton = new Button( sf::IntRect( 640 - 150, 360 - 40, 300, 80 ), sf::Color( 150, 150, 150 ), "Modify search", window->font );
}

void MainMenu::update()
{
	action = NO_ACTION;

	for ( auto click : window->clicks )
		if ( chooseButton->wasClicked( click ) )
			action = SEARCH_MENU;

    chooseButton->draw( window->window );
}