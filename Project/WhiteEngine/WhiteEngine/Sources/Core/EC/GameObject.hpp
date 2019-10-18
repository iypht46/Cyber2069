#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Components/Transform.hpp"

class Component;

class GameObject
{
protected:
	bool isActive;
	std::string m_objectName;

	int m_objectID;
	
	Transform m_transform;
	std::vector<Component*> m_components;

public:
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
	Component* GetComponent();
	//GameObject* GetGameObject();
	//void SetGameObject(GameObject* obj);
};