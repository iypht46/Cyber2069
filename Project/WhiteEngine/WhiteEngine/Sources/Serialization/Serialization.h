#pragma once

#include <string>
#include <fstream>
#include "Core/Logger.hpp"

#include <cereal/archives/binary.hpp>

#define ScenePath(path) "Sources/Assets/Scenes/" path ".scene"
#define AnimationControllerPath(path) "Sources/Assets/AnimationControllers/" path ".animcon"
#define PrefabPath(path) "Sources/Assets/Prefabs/" path ".prefab"
#define ParticlePath(path) "Sources/Assets/ParticleConfigs/" path ".ptcl"
#define DataPath "PlayerData.dat"

namespace Serialization {

	template <class T>
	void SaveObject(T& object, std::string path) {
		ENGINE_INFO("writing {}", path);

		std::ofstream file(path, std::ios::binary);

		cereal::BinaryOutputArchive oarchive(file);

		oarchive(object);
	}

	template <class T>
	bool LoadObject(T& object, std::string path) {
		ENGINE_INFO("loading {}", path);

		std::ifstream file(path, std::ios::binary);
		
		if (file.is_open()) {
			cereal::BinaryInputArchive iarchive(file);

			iarchive(object);

			return true;
		}

		return false;
	}
}