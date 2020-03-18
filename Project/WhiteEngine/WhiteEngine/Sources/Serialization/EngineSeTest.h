#pragma once
#include <string>

#include <cereal/types/base_class.hpp>

#include <Core/EC/GameObject.hpp>
#include "SomeClass.h"

//#include <cereal/access.hpp>
//==============
//serialazation test
//==============
	//template<class Archive>
	//void serialize(Archive& archive, GameObject& g) {
	//	archive(g.Name, g.isActive, g.outside, g.scv);
	//}

	template<class Archive>
	void serialize(Archive& ar, SomeBase& b) {
		ar(b.great);
	}

	template<class Archive>
	void serialize(Archive& ar, SomeClass& s) {
		ar(cereal::base_class<SomeBase>(&s), s.farr, s.v3);
	}

	void Save(GameObject& g) {
		std::ofstream file("srlztest.bin", std::ios::binary);
		cereal::BinaryOutputArchive oarchive(file);

		oarchive(g);
	}

	void Load(GameObject& g) {
		std::ifstream file("srlztest.bin", std::ios::binary);

		cereal::BinaryInputArchive iarchive(file);

		iarchive(g);
	}



CEREAL_REGISTER_TYPE(SomeClass);