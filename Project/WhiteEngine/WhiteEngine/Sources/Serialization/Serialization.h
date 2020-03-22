#pragma once

#include <string>
#include <fstream>
#include <cereal/archives/binary.hpp>

namespace Serialization {
	//#define SCENE_PATH "source/scene"

	template <class T>
	void SaveObject(T& object, std::string path) {
		std::ofstream file(path, std::ios::binary);

		//cereal::BinaryOutputArchive oarchive(file);

		//oarchive(object);
	}

	template <class T>
	void LoadObject(T& object, std::string path) {
		std::ifstream file(path, std::ios::binary);

		//cereal::BinaryInputArchive iarchive(file);

		//iarchive(object);
	}
}