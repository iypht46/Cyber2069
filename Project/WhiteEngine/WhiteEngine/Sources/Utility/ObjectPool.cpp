#include "ObjectPool.h"

void ObjectPool::AddObject(GameObject* obj) {
	m_objects.push_back(obj);
}

GameObject* ObjectPool::GetObject() {
	for (GameObject* obj : m_objects) {
		if (obj->Active()) {
			return obj;
		}
	}
	//not found
	return nullptr;
}