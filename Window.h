#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

// Forward declarations for different menu types
// (The header files rely on this file, so they can't be #included here)
class MainMenu;
class SearchMenu;

// A simple button with text that registers click events
// Defined here so all menu types have access to it
class Button
{
	sf::IntRect bounds;
	sf::RectangleShape shape;
	sf::Text text;

public:

	Button( sf::IntRect bounds, sf::Color color, std::string text, sf::Font& font );

	// Control methods
	bool wasClicked( sf::Vector2i clickPos );
	void draw( sf::RenderWindow* window );
};

// This struct holds the data that all menus should be able to easily access
// This exists in a separate object from 'Window' so a reference to it can be passed
// to the menus without passing a reference to the window itself, so that way, the
// window and menu objects don't mutually depend on each other's existence
struct WindowData
{
	sf::RenderWindow* window;
	sf::Font font;

	// Stores all clicks & key presses from this frame
	std::vector<sf::Vector2i> clicks;
	std::vector<sf::Keyboard::Key> keys;
};

class Window
{
	// State stores which menu is currently displayed
	enum State
	{
		MAIN_MENU,
		SEARCH_MENU
	};

	State state = MAIN_MENU;
	WindowData* windowData;

	// Different menus
	// A reference to them is stored even when they're not displayed/updated
	MainMenu* mainMenu;
	SearchMenu* searchMenu;

public:

	Window();

	// Control methods
	bool isOpen();
	void update();
};