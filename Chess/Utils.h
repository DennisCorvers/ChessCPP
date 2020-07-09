#pragma once

namespace Math {
	float clip(float value, float lower);

	float limit(float value, float upper);

	int limit(int value, int upper);

	float clamp(float value, float lower, float upper);

	float distance(sf::Vector2f v1, sf::Vector2f v2);

	float lerp(float a, float b, float t);

	sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t);

	bool approx(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& epsilon);

	bool approx(float a, float b, float epsilon);
}

inline sf::Vector2f operator/(const sf::Vector2f& a, const sf::Vector2f& b) {
	return sf::Vector2f(a.x / b.x, a.y / b.y);
}

namespace Graphics {
	void applyResize(sf::View& view, int windowWidth, int windowHeight);

	void applyResize(sf::View& view, const sf::Event& event);
}

