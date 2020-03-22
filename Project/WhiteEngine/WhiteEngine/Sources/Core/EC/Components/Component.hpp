#pragma once

#include <memory>

#include "Core/Factory.h"

#include <cereal/types/memory.hpp>
#include <cereal/cereal.hpp>

class GameObject;

class Component
{
protected:
	static int s_IDcounter;
	int m_componentID;

	bool enabled = true;
	GameObject* m_gameObject;

public:
	Component();
	int getComponentID();
	GameObject* GetGameObject();

	virtual void Init();
	virtual void SetGameObject(GameObject* obj);

	virtual ~Component() = 0;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			enabled
			);
	}
};