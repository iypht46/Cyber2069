#include "GameObject.hpp"

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