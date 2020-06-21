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