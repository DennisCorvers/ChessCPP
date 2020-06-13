#pragma once



namespace Math {
	float clip(float value, float lower) {
		return value < lower ? lower : value;
	}

	float limit(float value, float upper) {
		return value > upper ? upper : value;
	}

	int limit(int value, int upper) {
		return value > upper ? upper : value;
	}

	float clamp(float value, float lower, float upper) {
		return value < lower ? lower : value > upper ? upper : value;
	}
}