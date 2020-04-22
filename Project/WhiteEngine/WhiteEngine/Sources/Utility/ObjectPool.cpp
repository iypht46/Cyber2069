#include "ObjectPool.h"

void ObjectPool::Init() {
	if (prefabObjectPath != "") {
		for (int i = 0; i < objectCount; ++i) {
			AddObject(SceneManagement::Instantiate(prefabObjectPath).get());
		}
	}
	else {
		ENGINE_WARN("No Prefab Path");
	}
}

void ObjectPool::AddObject(GameObject* obj) {
	m_objects.push(obj);
}

void ObjectPool::SetActiveAllGameObject(bool active) 
{
	if (m_objects.size() > 0) {
		GameObject* back = m_objects.back();
		GameObject* obj = nullptr;

		do {
			obj = m_objects.front();

			m_objects.pop();
			m_objects.push(obj);

			obj->SetActive(active);

		} while (obj != back);

	}
}

int ObjectPool::GetPoolSize() {
	return m_objects.size();
}

//get obj from the queue regardless of active state
GameObject* ObjectPool::GetGameObject() {
	if (m_objects.size() > 0) {
		GameObject* obj = m_objects.front();
		m_objects.pop();
		m_objects.push(obj);

		return obj;
	}
	else {
		return nullptr;
	}
}

//get inactive obj from the queue, if none is inactive, return null
GameObject* ObjectPool::GetInactiveObject()
{
	if (m_objects.size() > 0) {
		GameObject* back = m_objects.back();
		GameObject* obj = nullptr;

		do {
			obj = m_objects.front();
			if (!obj->Active()) {
				m_objects.pop();
				m_objects.push(obj);

				return obj;
			}
			else {
				m_objects.pop();
				m_objects.push(obj);
			}

		} while (obj != back);

	}
	return nullptr;
}