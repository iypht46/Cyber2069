#pragma once

#include <string>
#include <fstream>
#include "Core/Logger.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

#define ScenePath(path) "Sources/Assets/Scenes/" path ".scene"
#define AnimationControllerPath(path) "Sources/Assets/AnimationControllers/" path ".animcon"
#define PrefabPath(path) "Sources/Assets/Prefabs/" path ".prefab"
#define ParticlePrefabPath(path) "Sources/Assets/Prefabs/ParticleObjects/" path ".prefab"

#define ParticlePath(path) "Sources/Assets/ParticleConfigs/" path ".ptcl"
#define TexturePath(path) "Sources/Assets/Sprites/" path ".png" 
#define SoundPath(path) "Sources/Assets/Sounds/SFX/" path ".wav"
#define MusicPath(path) "Sources/Assets/Sounds/Musics/" path ".wav"

#define DataPath(path) "Sources/Data/" path ".dat" 
#define XMLConfigPath(path) "Sources/Data/" path ".xml" 

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

	template <class T>
	void SaveObjectXML(T& object, std::string path) {
		ENGINE_INFO("writing xml {}", path);

		std::ofstream file(path);

		cereal::XMLOutputArchive oarchive(file);

		oarchive(CEREAL_NVP(object));
	}

	template <class T>
	bool LoadObjectXML(T& object, std::string path) {
		ENGINE_INFO("loading xml {}", path);

		std::ifstream file(path);

		if (file.is_open()) {
			cereal::XMLInputArchive iarchive(file);

			iarchive(object);

			return true;
		}

		return false;
	}
}