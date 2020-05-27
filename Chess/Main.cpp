#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include "Utilities.h"

#include "Mechanics.h"


using namespace Mechanics;

sf::Font font;

std::string floatToString(float number, int precision) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << number;
	return stream.str();
}

sf::Text createText(sf::String& string, unsigned int fontSize) {
	sf::Text text;
	text.setFont(font);
	text.setString(string);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(fontSize);

	return text;
}

int main()
{
	ChessPiece* piece = ChessPiece::createPiece(PieceColour::Black, PieceType::Knight);
	auto colour = piece->colour();
	auto type = piece->type();

	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	window.setVerticalSyncEnabled(true);

	if (!font.loadFromFile("Assets\\Fonts\\OpenSans-Regular.ttf")) {
		std::cout << "Unable to load font file" << std::endl;
		system("pause");
	}

	sf::Clock clock;
	float lastTime = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float currentTime = clock.restart().asSeconds();
		float fps = 1.0f / currentTime;
		lastTime = currentTime;

		sf::String str(floatToString(fps, 1));
		sf::Text text = createText(str, 18);

		window.clear();
		window.draw(text);
		window.display();
	}

	return 0;
}

