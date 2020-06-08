#pragma once



namespace Math {
	float clip(float value, float lower) {
		return value < lower ? lower : value;
	}

	float clamp(float value, float lower, float upper) {
		return value < lower ? lower : value > upper ? upper : value;
	}
}