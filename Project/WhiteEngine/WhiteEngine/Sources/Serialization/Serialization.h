#pragma once

#include <string>
#include <fstream>
#include <cereal/archives/binary.hpp>

#define ScenePath(path) "Sources/Assets/Scenes/" path ".scene"
#define AnimationControllerPath(path) "Sources/Assets/AnimationControllers/" path ".animcon"
#define PrefabPath(path) "Sources/Assets/Prefabs/" path ".prefab"
#define ParticlePath(path) "Sources/Assets/ParticleConfigs/" path ".ptcl"
#define SoundPath(path) "Sources/Assets/Sounds/SFX/" path ".wav"

namespace Serialization {

	template <class T>
	void SaveObject(T& object, std::string path) {
		std::ofstream file(path, std::ios::binary);

		cereal::BinaryOutputArchive oarchive(file);

		oarchive(object);
	}

	template <class T>
	void LoadObject(T& object, std::string path) {
		std::ifstream file(path, std::ios::binary);

		cereal::BinaryInputArchive iarchive(file);

		iarchive(object);
	}
}