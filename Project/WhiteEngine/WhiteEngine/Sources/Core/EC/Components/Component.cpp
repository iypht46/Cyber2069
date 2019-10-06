#include "Component.hpp"

GameObject* Component::GetGameObject() {
	return m_gameObject;
}

void Component::SetGameObject(GameObject* obj) {
	m_gameObject = obj;
}