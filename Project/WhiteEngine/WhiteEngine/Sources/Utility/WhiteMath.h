#pragma once

#include <glm/glm.hpp>

namespace WhiteMath {
	int Rand(int min, int max);
	float Rand(float min, float max);

	glm::vec2 interpolate(glm::vec2, glm::vec2, float t);
	glm::vec3 interpolate(glm::vec3, glm::vec3, float t);
	glm::vec4 interpolate(glm::vec4, glm::vec4, float t);

	//return percentage value/maxvalue within specified range
	float percentageValue(float value, float maxvalue, float offsetStart = 0.0f, float offsetEnd = 1.0f);
}