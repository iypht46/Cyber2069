#include "Component.hpp"

int Component::s_IDcounter = 0;

Component::Component() {
	Component::s_IDcounter++;
	m_componentID = s_IDcounter;
}

int Component::getComponentID() {
	return m_componentID;
}

GameObject* Component::GetGameObject() {
	return m_gameObject;
}

void Component::SetGameObject(GameObject* obj) {
	m_gameObject = obj;
}