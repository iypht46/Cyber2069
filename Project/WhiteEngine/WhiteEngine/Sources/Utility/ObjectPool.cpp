#include "ObjectPool.h"

void ObjectPool::AddObject(shared_ptr<GameObject> obj) {
	m_objects.push_back(obj);
}

int ObjectPool::GetPoolSize() {
	return m_objects.size();
}

shared_ptr<GameObject> ObjectPool::GetGameObject() {
	for (shared_ptr<GameObject> obj : m_objects) {
		if (obj->Active()) {
			return obj;
		}
	}
	//not found
	return nullptr;
}

shared_ptr<GameObject> ObjectPool::GetInactiveObject()
{
	for (shared_ptr<GameObject> obj : m_objects) {
		if (!obj->Active()) {
			return obj;
		}
	}
	//not found
	return nullptr;
}