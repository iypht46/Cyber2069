#include "SceneManager.h"

namespace SceneManagement {

	//static var declaration
	Scenes ScenePaths;
	std::unique_ptr<Scene> ActiveScene;

	void Scene::Init() {
		//init gameobjects
		for (std::pair<int, std::shared_ptr<GameObject>> objIt : GameObjectsInScene) {
			objIt.second->InitComponents();
		}

		//start gameobjects
		for (std::pair<int, std::shared_ptr<GameObject>> objIt : GameObjectsInScene) {
			objIt.second->StartComponents();
		}
	}

	std::shared_ptr<GameObject> Instantiate() {
		std::shared_ptr<GameObject> newObj = Factory<void, GameObject>::Create();
		SceneManagement::ActiveScene->GameObjectsInScene[newObj->GetID()] = newObj;

		return newObj;
	}

	std::shared_ptr<GameObject> Instantiate(std::string prefabPath) {
		std::shared_ptr<GameObject> newObj = Factory<void, GameObject>::Create(prefabPath);
		SceneManagement::ActiveScene->GameObjectsInScene[newObj->GetID()] = newObj;

		return newObj;
	}

	void LoadScene(std::string scenePath) {
		ActiveScene.reset(new Scene());

		Serialization::LoadObject<Scene>(*ActiveScene, scenePath);

		ActiveScene->Init();
	}

	void LoadScene(int sceneIndex) {
		LoadScene(ScenePaths[sceneIndex]);
	}

	void UnloadCurrentScene() { }
}