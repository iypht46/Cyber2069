#include "SceneManager.h"

namespace SceneManagement {

	void LoadScene(std::string scenePath) {
		ActiveScene.reset(new Scene());

		//Serialization::LoadObject<Scene>(*ActiveScene, scenePath);

		ActiveScene->Init();
	}

	void LoadScene(int sceneIndex) {
		LoadScene(ScenePaths[sceneIndex]);
	}

	void UnloadCurrentScene() { }
}