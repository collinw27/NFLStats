#include <SFML/Graphics.hpp>

class Button
{
	sf::IntRect bounds;
	sf::RectangleShape shape;
	sf::Text text;

public:

	Button( sf::IntRect bounds, sf::Color color, std::string text, sf::Font& font );

	void draw( sf::RenderWindow* window );
};