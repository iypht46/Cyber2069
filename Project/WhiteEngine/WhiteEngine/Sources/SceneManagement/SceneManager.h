#pragma once

#include <memory>
#include <map>
#include <string>

#include "Core/EC/GameObject.hpp"
#include "Serialization/Serialization.h"

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>

class GameObject;

namespace SceneManagement {
	class Scene
	{
	public:
		std::map<int, std::shared_ptr<GameObject>> GameObjectsInScene;

		//call init/awake/start on component assign gameobject assing collider/rigid body to physics scene
		void Init();

		//serialization
	public:
		template<class Archive>
		void serialize(Archive& archive) {
			archive(
				GameObjectsInScene
				);

			archive.serializeDeferments();
		}
	};


	using Scenes = std::map<int, std::string>;

	extern Scenes ScenePaths;
	extern std::unique_ptr<Scene> ActiveScene;

	extern std::shared_ptr<GameObject> Instantiate();
	extern std::shared_ptr<GameObject> Instantiate(std::string prefabPath);

	void LoadScene(std::string scenePath);
	void LoadScene(int sceneIndex);
	void UnloadCurrentScene();
}