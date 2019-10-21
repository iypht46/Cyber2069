#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Components/Transform.hpp"
#include "Core/Factory.h"

class Component;

class GameObject
{
protected:
	bool isActive;
	std::string m_objectName;

	int m_objectID;
	
	std::vector<Component*> m_components;

public:
	Transform m_transform;

	void SetActive(bool activestate);

	virtual void OnAwake() {};
	virtual void OnEnable() {};
	virtual void OnStart() {};
	virtual void OnUpdate(float dt) {};
	virtual void OnFixedUpdate(float dt) {};
	virtual void OnDisable() {};

	template <class T>
	void AddComponent();

	template <class T>
	T* GetComponent();
	//GameObject* GetGameObject();
	//void SetGameObject(GameObject* obj);
};

template<class T>
void GameObject::AddComponent() {
	m_components.push_back(Factory<T>::Create());
	m_components.back()->SetGameObject(this);
}

template<class T>
T* GameObject::GetComponent() {

	for (Component* component : m_components) {
		if (dynamic_cast<T*>(component)) {
			T* tmp = dynamic_cast<T*>(component);
			return tmp;
		}
	}

	return nullptr;
}