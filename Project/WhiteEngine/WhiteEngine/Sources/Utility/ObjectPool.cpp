#include "ObjectPool.h"

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