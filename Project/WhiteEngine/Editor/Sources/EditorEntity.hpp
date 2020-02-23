#pragma once
#include <memory>
#include <vector>

#include "Core/Factory.h"
#include "Core/EC/GameObject.hpp"
#include "EditorComponent.hpp"

namespace Tools
{
	class PrefabEditor;
	using GameObjectHandle = UNIQUE_HANDLE(GameObject);
	using ComponentHandle = UNIQUE_HANDLE(EditorComponent);
	using TransformHandle = UNIQUE_HANDLE(TransformEC);
	//using MeshRendererHandle = UNIQUE_HANDLE(MeshRendererEC);
	//using ComponentList = std::vector<std::unique_ptr<EditorComponent>>;

	class EditorEntity
	{
		friend class PrefabEditor;
		friend class Inspector;
	private:
		std::string* m_objectName;
		int* m_objectID;
		bool* m_isActive;
		GameObjectHandle m_gameObject;
		std::vector<EditorComponent*> m_components;

	public:
		EditorEntity();
		EditorEntity(GameObject* gameObject);
		
		//Interface
		void AddComponent(const char*);
		std::vector<EditorComponent*>& GetComponentList();
		std::string GetName();
		int GetID();
	};
}