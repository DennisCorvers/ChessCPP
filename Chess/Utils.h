#pragma once

namespace Math {
	float clip(float value, float lower);

	float limit(float value, float upper);

	int limit(int value, int upper);

	float clamp(float value, float lower, float upper);

	float distance(sf::Vector2f v1, sf::Vector2f v2);
}

