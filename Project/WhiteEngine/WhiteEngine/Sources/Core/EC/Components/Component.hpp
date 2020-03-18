#pragma once

#include <cereal/archives/binary.hpp>

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
	virtual void SetGameObject(GameObject* obj);

	virtual ~Component() = 0;

private:
	//serialization
	template<class Archive>
	void serialize(Archive& archive) {
		archive(enabled);
	}
};