#pragma once

#include <memory>

#include "Core/Factory.h"

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>

class GameObject;

class Component
{
protected:
	static int s_IDcounter;
	int m_componentID;

	GameObject* m_gameObject = nullptr;

public:
	bool enabled = true;

	Component();
	int getComponentID();
	GameObject* GetGameObject();

	virtual void Init();
	void SetGameObject(GameObject* obj);

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