#pragma once

#include <set>
#include <memory>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/set.hpp>

class GameObject;

namespace SceneManagement {
	class Scene
	{
	private:
		int a;
		std::set<std::shared_ptr<GameObject>> GameObjects;
	public:

		//call init/awake/start on component assign collider/rigid body to physics scene
		void Init(void) {}

		//serialization
	public:
		template<class Archive>
		void serialize(Archive& archive) {
			archive(
				a
				//GameObjects
				);

			//archive.serializeDeferments();
		}
	};
}