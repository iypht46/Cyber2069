#pragma once
#include <memory>
#include <vector>

#include "Core/Factory.h"
#include "Core/EC/GameObject.hpp"
#include "EditorComponent.hpp"

namespace Tools
{
	class PrefabEditor;
	using GameObjectHandle = std::unique_ptr<GameObject>;
	using ComponentHandle = std::unique_ptr<EditorComponent>;
	using ComponentList = std::vector<ComponentHandle>;

	class EditorEntity
	{
		friend class PrefabEditor;
	private:
		std::string m_objectName;
		int m_objectID;
		GameObjectHandle m_gameObject;
		ComponentList m_components;

	public:
		EditorEntity();
		
		//Interface
		void AddComponent(const char*);
		ComponentList& GetComponentList();
		std::string GetName();
		int GetID();
	};
}