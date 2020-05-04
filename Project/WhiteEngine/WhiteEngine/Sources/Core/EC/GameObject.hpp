#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Components/Transform.hpp"
#include "Components/BehaviourScript.h"
#include "Core/Factory.h"
#include "Core/LogCustomType.hpp"
#include "Physic/PhysicScene.hpp"

//Forward Declaration
namespace Physic { struct Collision; }
namespace Tools { class EditorEntity; }

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

class GameObject
{
protected:
	friend class BehaviourScript;
	friend class Collider;
	friend class Tools::EditorEntity;

	std::string m_objectName = "GameObject";
	bool isActive = true;

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
	~GameObject() {}

	std::string Layer = "Default";

	std::string Name = "GameObj";
	//change this to smart ptr
	std::shared_ptr<Transform> m_transform;

	void SetActive(bool activestate);
	bool Active();

	std::string GetName();
	void SetName(std::string);

	void AddComponent(Component*);
	int GetID();

	//init all member component
	void SetAllComponents();
	void InitComponents();

	//start all member behaviour script
	void StartComponents();

	template <class T>
	T* AddComponent();

	template <class T>
	std::weak_ptr<T> AddComponent_weak();

	template <class T>
	T* GetComponent();

	template <class T>
	std::weak_ptr<T> GetComponent_weak();

	//Log to logger
	LogCustomType_DC(GameObject);

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			isActive,
			m_objectName,
			Layer,
			m_transform,
			m_components
			);
	}
};

template<class T>
T* GameObject::AddComponent() {
	std::shared_ptr<T> component = Factory<Component, T>::Create();

	m_components.push_back(component);
	m_components.back()->SetGameObject(this);

	return component.get();
}

//return a weak ptr of component instead of raw
template<class T>
std::weak_ptr<T> GameObject::AddComponent_weak() {
	std::shared_ptr<T> component = Factory<Component, T>::Create();

	m_components.push_back(component);
	m_components.back()->SetGameObject(this);

	return component;
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

//return a weak ptr of component instead of raw
template<class T>
std::weak_ptr<T> GameObject::GetComponent_weak() {

	for (std::shared_ptr<Component> component : m_components)
	{
		if (dynamic_pointer_cast<T>(component))
		{
			return dynamic_pointer_cast<T>(component);
		}
	}

	return nullptr;
}

LogCustomType_DF(GameObject)
{
	return os << "GameObject: " << obj.m_objectName << "\n";
}
