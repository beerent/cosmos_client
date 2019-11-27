#pragma once
#include <math.h>
#include "glm/glm.hpp"

namespace Math {
	static float LookAt2d(vec2 position, vec2 direction, vec2 target) {
		vec2 current = direction - position;
		vec2 targetV = target - position;
		current.Normalize();
		targetV.Normalize();
		float dotProduct = current.Dot(targetV);
		float r1 = acos(dotProduct);
		return r1;
	}
}