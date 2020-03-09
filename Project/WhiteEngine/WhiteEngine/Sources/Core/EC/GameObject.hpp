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

//cereal test
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/access.hpp>
#include "Serialization/SomeClass.h"
//cereal test

template<class Archive>
void serialize(Archive& archive, GameObject& g);

class GameObject
{
protected:
	friend class BehaviourScript;
	friend class Collider;
	template<class Archive>
	friend void serialize(Archive& archive, GameObject& g);

	bool isActive;

	static int s_IDCounter;
	int m_objectID;
	
	std::vector<shared_ptr<Component>> m_components;
	std::vector<shared_ptr<BehaviourScript>> m_scripts;

	void CollisionEnter(const Physic::Collision);
	void CollisionStay(const Physic::Collision);
	void CollisionExit(const Physic::Collision);
	void TriggerEnter(const Physic::Collision);
	void TriggerStay(const Physic::Collision);
	void TriggerExit(const Physic::Collision);
public:
	GameObject();
	std::string Name;
	Transform m_transform;

	void SetActive(bool activestate);
	bool Active();

	int GetID();

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

	//===========================
	////test serialzation
	//template<class Archive>
	//void serialize(Archive &archive);

	//void Save();
	//void Load();
	shared_ptr<SomeBase> outside;
	std::vector<shared_ptr<SomeBase>> scv;

	//===========================

	//GameObject* GetGameObject();
	//void SetGameObject(GameObject* obj);
};

template<class T>
T* GameObject::AddComponent() {
	shared_ptr<T> component = Factory<T>::Create();

	m_components.push_back(component);
	m_components.back()->SetGameObject(this);

	//if is behaviou script, also assign to script collection
	shared_ptr<BehaviourScript> behaviour = dynamic_pointer_cast<BehaviourScript>(component);
	if (behaviour) {
		m_scripts.push_back(behaviour);
	}

	return component.get();
}

template<class T>
T* GameObject::GetComponent() {

	for (shared_ptr<Component> component : m_components) 
	{
		if (dynamic_pointer_cast<T>(component))
		{
			return dynamic_pointer_cast<T>(component).get();
		}
	}
	
	return nullptr;
}

LogCustomType_DF(GameObject)
{
	return os << "GameObject: " << obj.Name << "\n";
}