#include "Core/Factory.h"

int Component::s_IDcounter = 0;

Component::Component() {
	Component::s_IDcounter++;
	m_componentID = s_IDcounter;

	Factory<Component, Component>::Add(this);
}

void Component::Init() { }

int Component::getComponentID() {
	return m_componentID;
}

GameObject* Component::GetGameObject() {
	return m_gameObject;
}

void Component::SetGameObject(GameObject* obj) {
	m_gameObject = obj;
}

Component::~Component() {
	Factory<Component, Component>::Remove(this);
}