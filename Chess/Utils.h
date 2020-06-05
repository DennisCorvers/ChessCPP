#pragma once

namespace Math {
	float clip(float value, float upper) {
		return value < upper ? upper : value;
	}
}