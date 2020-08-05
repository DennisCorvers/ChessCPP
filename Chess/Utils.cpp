#include "pch.h"
#include <cctype>

namespace Math {
	int limit(int value, int upper) {
		return value > upper ? upper : value;
	}

	int clamp(int value, int lower, int upper) {
		return value < lower ? lower : value > upper ? upper : value;
	}

	int clip(int value, int lower) {
		return value < lower ? lower : value;
	}
}
namespace Mathf {
	float clip(float value, float lower) {
		return value < lower ? lower : value;
	}

	float limit(float value, float upper) {
		return value > upper ? upper : value;
	}

	float clamp(float value, float lower, float upper) {
		return value < lower ? lower : value > upper ? upper : value;
	}

	float distance(sf::Vector2f v1, sf::Vector2f v2) {
		return std::sqrt(
			std::pow(v1.x - v2.x, 2) +
			std::pow(v1.y - v2.y, 2));
	}

	float lerp(float a, float b, float t) {
		return a * (1.f - t) + b * t;
	}

	sf::Vector2f lerp(const sf::Vector2f & a, const sf::Vector2f & b, float t) {
		return sf::Vector2f(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
	}

	bool approx(const sf::Vector2f & a, const sf::Vector2f & b, const sf::Vector2f & epsilon) {
		return approx(a.x, b.x, epsilon.x) && approx(a.y, b.y, epsilon.y);
	}

	bool approx(float a, float b, float epsilon) {
		return std::abs(a - b) < epsilon;
	}
}

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

	sf::Vector2u clampWindow(sf::RenderWindow & window, sf::Vector2i minSize)
	{
		auto& a = window;
		sf::Vector2u newSize;

		newSize.x = Math::clip(window.getSize().x, minSize.x);
		newSize.y = Math::clip(window.getSize().y, minSize.y);

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
	return false;
}
