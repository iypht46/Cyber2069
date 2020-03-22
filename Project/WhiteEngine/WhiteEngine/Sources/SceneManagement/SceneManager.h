#pragma once

#include <memory>
#include <map>
#include <string>

#include "Scene.h"
#include "Serialization/Serialization.h"

namespace SceneManagement {

	using Scenes = std::map<int, std::string>;

	Scenes ScenePaths;
	std::unique_ptr<Scene> ActiveScene;

	void LoadScene(std::string scenePath);
	void LoadScene(int sceneIndex);
	void UnloadCurrentScene();
}