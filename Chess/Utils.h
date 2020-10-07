#pragma once

namespace Math {
	int limit(int value, int upper);

	int clamp(int value, int lower, int upper);

	int clip(int value, int lower);
}

namespace Mathf {
	float clip(float value, float lower);

	float limit(float value, float upper);

	float clamp(float value, float lower, float upper);

	float distance(sf::Vector2f v1, sf::Vector2f v2);

	float lerp(float a, float b, float t);

	sf::Vector2f lerp(const sf::Vector2f & a, const sf::Vector2f & b, float t);

	bool approx(const sf::Vector2f & a, const sf::Vector2f & b, const sf::Vector2f & epsilon);

	bool approx(float a, float b, float epsilon);
}

namespace Graphics {
	const sf::Vector2i DEFAULT_SIZE = sf::Vector2i(1920, 1027);
	const sf::Vector2i MIN_SIZE = sf::Vector2i(DEFAULT_SIZE.x / 2, DEFAULT_SIZE.y / 2);
	const int FPS_LIMIT = 60;
	const int FPS_INACTIVE = 15;

	void applyResize(sf::View& view, int windowWidth, int windowHeight);

	void applyResize(sf::View& view, const sf::Event& event);

	sf::Vector2u clampWindow(sf::RenderWindow& window, sf::Vector2i minSize);
}

inline sf::Vector2f operator/(const sf::Vector2f& a, const sf::Vector2f& b) {
	return sf::Vector2f(a.x / b.x, a.y / b.y);
}

inline bool operator < (const sf::Vector2i& a, const sf::Vector2i& b) {
	return a.x < b.x && a.y << b.y;
}

namespace String {
	std::string toLower(const std::string& str);
	bool stob(const std::string& str);
}



