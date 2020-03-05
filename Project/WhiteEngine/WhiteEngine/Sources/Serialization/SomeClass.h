#pragma once
#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/array.hpp>
#include <Serialization/glmCereal.h>
#include "Core/Logger.hpp"

class SomeClass{
public:
	int farr;
	glm::vec3 v3;
	shared_ptr<SomeClass> inside = nullptr;

	SomeClass() :farr(0) { v3 = glm::vec3(0); }

	template<class Archive>
	void serialize(Archive &ar) {
		ar(farr, v3, cereal::defer(inside));
	}

	void out() {
		ENGINE_INFO("somclass farr:{} vec:{},{},{}", farr, v3.x, v3.y, v3.z);
		if (inside != nullptr) {
			ENGINE_INFO("someclass inside farr:{} vec:{},{},{}", inside->farr, inside->v3.x, inside->v3.y, inside->v3.z);
		}
		else {
			ENGINE_INFO("nothing inside");
		}
	}
};