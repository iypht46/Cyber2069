#include "WhiteMath.h"
#include <cstdlib>

namespace WhiteMath {
	int Rand(int min, int max) {
		return min + (rand() % (max - min));
	}

	float Rand(float min, float max) {
		return min + rand() / (RAND_MAX / (max - min));
	}

	glm::vec2 interpolate(glm::vec2 a, glm::vec2 b, float t) {
		return ((1.0f - t) * a) + (t * b);
	}

	glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float t) {
		return ((1.0f - t) * a) + (t * b);
	}

	glm::vec4 interpolate(glm::vec4 a, glm::vec4 b, float t) {
		return ((1.0f - t) * a) + (t * b);
	}

	float percentageValue(float value, float maxvalue, float offsetStart, float offsetEnd) {
		offsetStart = glm::clamp(offsetStart, 0.0f, 1.0f);
		offsetEnd = glm::clamp(offsetEnd, 0.0f, 1.0f);
		return glm::clamp((value - (offsetStart * maxvalue)) / ((offsetEnd - offsetStart) * maxvalue), 0.0f, 1.0f);
	}
}