#include "Game.h"


//sf::Font font;
//
//std::string floatToString(float number, int precision) {
//	std::stringstream stream;
//	stream << std::fixed << std::setprecision(precision) << number;
//	return stream.str();
//}
//
//sf::Text createText(sf::String& string, unsigned int fontSize) {
//	sf::Text text;
//	text.setFont(font);
//	text.setString(string);
//	text.setFillColor(sf::Color::White);
//	text.setCharacterSize(fontSize);
//
//	return text;
//}

int main()
{
	Game game;
	while (game.GetWindow()->isOpen()) {
		game.update();
		game.render();
		game.lateUpdate();
	}

	return 0;
}


