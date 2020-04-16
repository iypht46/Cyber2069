#pragma once
#include <queue>
#include <memory>
#include "SceneManagement/SceneManager.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

//template <class OBJ>
class ObjectPool :public Component {
private:
	std::queue<GameObject*> m_objects;
	int lastReturnedObject = 0;
public:
	std::string prefabObjectPath;
	int objectCount;

	virtual void Init();

	//manually add object to pool
	void AddObject(GameObject*);
	int GetPoolSize();

	//get gameobject cycling through all object from pool,
	GameObject* GetGameObject();

	//get inactive gameobject from pool
	GameObject* GetInactiveObject();

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			prefabObjectPath,
			objectCount
			);
	}
};

CEREAL_REGISTER_TYPE(ObjectPool);