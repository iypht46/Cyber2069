#pragma once

class GameObject;

class Component
{
protected:
	bool enabled = true;

	static int s_IDcounter;
	int m_componentID;
	GameObject* m_gameObject;

public:
	Component();
	int getComponentID();

	GameObject* GetGameObject();
	void SetEnable(bool enable);
	virtual void SetGameObject(GameObject* obj);

	virtual ~Component() = 0;
};