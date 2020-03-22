#include "ObjectPool.h"

void ObjectPool::AddObject(shared_ptr<GameObject> obj) {
	m_objects.push_back(obj);
}

int ObjectPool::GetPoolSize() {
	return m_objects.size();
}

GameObject* ObjectPool::GetGameObject() {
	for (shared_ptr<GameObject> obj : m_objects) {
		if (obj->Active()) {
			return obj.get();
		}
	}
	//not found
	return nullptr;
}

GameObject* ObjectPool::GetInactiveObject()
{
	for (shared_ptr<GameObject> obj : m_objects) {
		if (!obj->Active()) {
			return obj.get();
		}
	}
	//not found
	return nullptr;
}