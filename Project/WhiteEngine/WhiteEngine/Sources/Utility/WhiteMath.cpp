#include "WhiteMath.h"
#include <cstdlib>

namespace WhiteMath {
	int Rand(int min, int max) {
		return min + (rand() % (max - min));
	}

	float Rand(float min, float max) {
		return min + rand() / (RAND_MAX / (max - min));
	}
}