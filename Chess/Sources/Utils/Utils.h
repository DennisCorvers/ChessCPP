#pragma once

namespace Graphics {
	const sf::Vector2u DEFAULT_SIZE = sf::Vector2u(1920, 1027);
	const sf::Vector2u MIN_SIZE = sf::Vector2u(DEFAULT_SIZE.x / 2, DEFAULT_SIZE.y / 2);
	constexpr int FPS_LIMIT = 60;
	constexpr int FPS_INACTIVE = 15;

	void applyResize(sf::View& view, int windowWidth, int windowHeight);

	void applyResize(sf::View& view, const sf::Event& event);

	sf::Vector2u clampWindow(sf::RenderWindow& window, sf::Vector2u minSize);
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



