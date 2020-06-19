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
