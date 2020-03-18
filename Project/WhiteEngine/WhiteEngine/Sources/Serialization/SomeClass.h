#pragma once
#include <glm/glm.hpp>
#include "Core/Logger.hpp"

class SomeBase {
public:
	int great = 0;

	virtual void out() = 0;


};

class SomeClass : public SomeBase {
public:
	int farr;
	glm::vec3 v3;

	SomeClass() :farr(0) { v3 = glm::vec3(0); }



	virtual void out() {
		ENGINE_INFO("somclass great:{} farr:{} vec:{},{},{}", great, farr, v3.x, v3.y, v3.z);
	}
};

