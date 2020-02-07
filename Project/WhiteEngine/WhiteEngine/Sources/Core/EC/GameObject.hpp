#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Components/Transform.hpp"
#include "Core/Factory.h"
#include "Core/LogCustomType.hpp"

//Forward Declaration
class Component;
class BehaviourScript;
namespace Physic { struct Collision; }

class GameObject
{
protected:
	friend class BehaviourScript;
	friend class Collider;

	bool isActive;
	std::string m_objectName;

	int m_objectID;
	
	std::vector<Component*> m_components;
	std::vector<BehaviourScript*> m_scripts;

	void CollisionEnter(const Physic::Collision);
	void CollisionStay(const Physic::Collision);
	void CollisionExit(const Physic::Collision);
	void TriggerEnter(const Physic::Collision);
	void TriggerStay(const Physic::Collision);
	void TriggerExit(const Physic::Collision);
public:
	GameObject();

	Transform m_transform;

	void SetActive(bool activestate);
	bool Active();

	virtual void OnAwake() {};
	virtual void OnEnable() {};
	virtual void OnStart() {};
	virtual void OnUpdate(float dt) {};
	virtual void OnFixedUpdate(float dt) {};
	virtual void OnDisable() {};

	template <class T>
	T* AddComponent();

	template <class T>
	T* GetComponent();

	//Log to logger
	LogCustomType_DC(GameObject);

	//GameObject* GetGameObject();
	//void SetGameObject(GameObject* obj);
};

template<class T>
T* GameObject::AddComponent() {
	T* component = Factory<T>::Create();
	m_components.push_back(component);
	m_components.back()->SetGameObject(this);

	return component;
}

template<class T>
T* GameObject::GetComponent() {

	for (Component* component : m_components) 
	{
		if (dynamic_cast<T*>(component))
		{
			return dynamic_cast<T*>(component);
		}
	}
	
	return nullptr;
}

LogCustomType_DF(GameObject)
{
	return os << "GameObject: " << obj.m_objectName << "\n";
}

