#pragma once
#include "../GameObject.hpp"

class GameObject;

class Component
{
protected:
	static int s_IDcounter;
	int m_componentID;
	GameObject* m_gameObject;

public:
	virtual void OnAwake() = 0;
	virtual void OnEnable() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnFixedUpdate(float dt) = 0;
	virtual void OnDisable() = 0;

	GameObject* GetGameObject();
	void SetGameObject(GameObject* obj);
};