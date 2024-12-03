#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class MainMenu;
class SearchMenu;

class Button
{
	sf::IntRect bounds;
	sf::RectangleShape shape;
	sf::Text text;

public:

	Button( sf::IntRect bounds, sf::Color color, std::string text, sf::Font& font );

	bool wasClicked( sf::Vector2i clickPos );
	void draw( sf::RenderWindow* window );
};

struct WindowData
{
	sf::RenderWindow* window;
	sf::Font font;
	std::vector<sf::Vector2i> clicks;
	std::vector<sf::Keyboard::Key> keys;
};

class Window
{
	enum State
	{
		MAIN_MENU,
		SEARCH_MENU
	};

	State state = MAIN_MENU;
	WindowData* windowData;

	MainMenu* mainMenu;
	SearchMenu* searchMenu;

public:

	Window();

	bool isOpen();
	void update();
};