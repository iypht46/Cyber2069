#include "GameObject.hpp"
#include "../Factory.h"

template<class T>
void GameObject::AddComponent() {
	Factory<T>::Create();
}

template<class T>
Component* GameObject::GetComponent() {
	for (Component* component : m_components) {
		if (dynamic_cast<T>(component)) {
			return component;
		}
	}
}

void GameObject::SetActive(bool activestate) {
	isActive = activestate;
}