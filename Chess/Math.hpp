#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

namespace Math {

	template<typename T>
	const T& min(const T& a, const T& b) {
		return (b < a) ? b : a;
	}

	template<typename T>
	const T& max(const T& a, const T& b) {
		return (a < b) ? b : a;
	}

	template<typename T>
	T clamp(const T& value, const T& lower, const T& upper) {
		return max(lower, min(value, upper));
	}

	template<typename T>
	float distance(sf::Vector2<T> v1, sf::Vector2<T> v2) {
		return std::sqrt(
			std::pow(v1.x - v2.x, 2) +
			std::pow(v1.y - v2.y, 2));
	}

	template<typename T>
	T lerp(const T& a, const T& b, const T& t) {
		return a * (1.f - t) + b * t;
	}

	template<typename T>
	sf::Vector2<T> lerp(const sf::Vector2<T> & a, const sf::Vector2<T> & b, float t) {
		return sf::Vector2<T>(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
	}

	template<typename T>
	bool approx(const sf::Vector2<T> & a, const sf::Vector2<T> & b, const sf::Vector2<T> & epsilon) {
		return approx(a.x, b.x, epsilon.x) && approx(a.y, b.y, epsilon.y);
	}

	template<typename T>
	bool approx(const T& a, const T& b, const T& epsilon) {
		return std::abs(a - b) < epsilon;
	}
}
