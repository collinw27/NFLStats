#include <SFML/Graphics.hpp>
#include "Widgets.h"

class Window
{
	sf::RenderWindow* window;
	sf::Font font;

	Button* testButton;

public:

	Window();

	bool isOpen();
	void update();
};