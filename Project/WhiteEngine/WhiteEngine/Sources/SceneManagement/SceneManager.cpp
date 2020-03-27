#include "SceneManager.h"

namespace SceneManagement {

	//static var declaration
	Scenes ScenePaths;
	std::unique_ptr<Scene> ActiveScene;

	void Scene::Init() {
		//init gameobjects
		for (std::shared_ptr<GameObject> obj : GameObjectsInScene) {
			obj->InitComponents();
		}
	}

	GameObject* Instantiate() {
		std::shared_ptr<GameObject> newObj = Factory<GameObject>::Create();
		SceneManagement::ActiveScene->GameObjectsInScene.insert(newObj);

		return newObj.get();
	}

	GameObject* Instantiate(std::string prefabPath) {
		std::shared_ptr<GameObject> newObj = Factory<GameObject>::Create(prefabPath);
		SceneManagement::ActiveScene->GameObjectsInScene.insert(newObj);

		newObj->InitComponents();

		return newObj.get();
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