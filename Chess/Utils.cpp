#include "pch.h"
#include <cctype>

namespace Graphics {
	void applyResize(sf::View & view, int windowWidth, int windowHeight)
	{
		float windowRatio = windowWidth / (float)windowHeight;
		float viewRatio = view.getSize().x / (float)view.getSize().y;
		float sizeX = 1;
		float sizeY = 1;
		float posX = 0;
		float posY = 0;

		bool horizontalSpacing = true;
		if (windowRatio < viewRatio)
			horizontalSpacing = false;

		if (horizontalSpacing) {
			sizeX = viewRatio / windowRatio;
			posX = (1 - sizeX) / 2.f;
		}

		else {
			sizeY = windowRatio / viewRatio;
			posY = (1 - sizeY) / 2.f;
		}

		view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
	}

	void applyResize(sf::View & view, const sf::Event & event) {
		applyResize(view, event.size.width, event.size.height);
	}

	sf::Vector2u clampWindow(sf::RenderWindow & window, sf::Vector2u minSize)
	{
		auto& a = window;
		sf::Vector2u newSize;

		newSize.x = Math::min(window.getSize().x, minSize.x);
		newSize.y = Math::min(window.getSize().y, minSize.y);

		window.setSize(newSize);
		return window.getSize();
	}
}

std::string String::toLower(const std::string & str)
{
	std::string low(str);
	std::transform(low.begin(), low.end(), low.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);

	return low;
}

bool String::stob(const std::string & str)
{
	std::string low = toLower(str);
	if (!low.compare("true"))
		return true;
	if (!low.compare("false"))
		return false;

	throw new std::invalid_argument("Conversion to bool not possible.");
}
