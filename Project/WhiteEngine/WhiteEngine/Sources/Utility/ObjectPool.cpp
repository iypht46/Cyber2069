#include "ObjectPool.h"

void ObjectPool::Init() {
	if (prefabObjectPath != "") {
		for (int i = 0; i < objectCount; ++i) {
			AddObject(SceneManagement::Instantiate(prefabObjectPath));
		}
	}
	else {
		ENGINE_WARN("No Prefab Path");
	}
}

void ObjectPool::AddObject(GameObject* obj) {
	m_objects.push_back(obj);
}

int ObjectPool::GetPoolSize() {
	return m_objects.size();
}

GameObject* ObjectPool::GetGameObject() {
	for (GameObject* obj : m_objects) {
		if (obj->Active()) {
			return obj;
		}
	}
	//not found
	return nullptr;
}

GameObject* ObjectPool::GetInactiveObject()
{
	for (GameObject* obj : m_objects) {
		if (!obj->Active()) {
			return obj;
		}
	}
	//not found
	return nullptr;
}