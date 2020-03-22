#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Components/Transform.hpp"
#include "Components/BehaviourScript.h"
#include "Core/Factory.h"
#include "Core/LogCustomType.hpp"

//Forward Declaration
namespace Physic { struct Collision; }

//cereal test
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include "Serialization/SomeClass.h"
//cereal test

class GameObject
{
protected:
	friend class BehaviourScript;
	friend class Collider;

	bool isActive;

	static int s_IDCounter;
	int m_objectID;

	std::vector<std::shared_ptr<Component>> m_components;
	std::vector<std::shared_ptr<BehaviourScript>> m_scripts;

	void CollisionEnter(const Physic::Collision);
	void CollisionStay(const Physic::Collision);
	void CollisionExit(const Physic::Collision);
	void TriggerEnter(const Physic::Collision);
	void TriggerStay(const Physic::Collision);
	void TriggerExit(const Physic::Collision);
public:
	GameObject();
	std::string Name;
	//change this to smart ptr
	std::shared_ptr<Transform> m_transform;

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
	//shared_ptr<SomeBase> outside;
	//std::vector<shared_ptr<SomeBase>> scv;

	//===========================

	//GameObject* GetGameObject();
	//void SetGameObject(GameObject* obj);

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			isActive,
			Name,
			m_transform,
			m_components,
			cereal::defer(m_scripts)
			);
	}
};

template<class T>
T* GameObject::AddComponent() {
	std::shared_ptr<T> component = Factory<T>::Create();

	m_components.push_back(component);
	m_components.back()->SetGameObject(this);

	//if is behaviou script, also assign to script collection
	std::shared_ptr<BehaviourScript> behaviour = dynamic_pointer_cast<BehaviourScript>(component);
	if (behaviour) {
		m_scripts.push_back(behaviour);
	}

	return component.get();
}

template<class T>
T* GameObject::GetComponent() {

	for (std::shared_ptr<Component> component : m_components)
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
