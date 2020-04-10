#pragma once
#include <vector>
#include <memory>
#include "SceneManagement/SceneManager.h"

#include <cereal/types/string.hpp>

//template <class OBJ>
class ObjectPool :Component {
private:
	std::vector<GameObject*> m_objects;
public:
	std::string prefabObjectPath;
	int objectCount;

	virtual void Init();

	//manually add object to pool
	void AddObject(GameObject*);
	int GetPoolSize();

	//get active gameobject from pool
	GameObject* GetGameObject();

	//get inactive gameobject from pool
	GameObject* GetInactiveObject();

	//get all gameobject from pool
	std::vector<GameObject*> GetAllGameObject() { return m_objects; }

	//serialization
private:
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