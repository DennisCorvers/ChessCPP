#include "pch.h"

float Math::clip(float value, float lower) {
	return value < lower ? lower : value;
}

float Math::limit(float value, float upper) {
	return value > upper ? upper : value;
}

int Math::limit(int value, int upper) {
	return value > upper ? upper : value;
}

float Math::clamp(float value, float lower, float upper) {
	return value < lower ? lower : value > upper ? upper : value;
}

float Math::distance(sf::Vector2f v1, sf::Vector2f v2) {
	return std::sqrtf(
		std::powf(v1.x - v2.x, 2) +
		std::powf(v1.y - v2.y, 2));
}

float Math::lerp(float a, float b, float t) {
	return a * (1.f - t) + b * t;
}

sf::Vector2f Math::lerp(const sf::Vector2f & a, const sf::Vector2f & b, float t) {
	return sf::Vector2f(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
}

bool Math::approx(const sf::Vector2f & a, const sf::Vector2f & b, const sf::Vector2f & epsilon) {
	return approx(a.x, b.x, epsilon.x) && approx(a.y, b.y, epsilon.y);
}

bool Math::approx(float a, float b, float epsilon) {
	return std::abs(a - b) < epsilon;
}
