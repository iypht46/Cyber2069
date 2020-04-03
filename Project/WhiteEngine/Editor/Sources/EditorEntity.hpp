#pragma once
//White Engine
#include "Core/Factory.h"
#include "Core/EC/GameObject.hpp"
#include "Core/Handle.hpp"
//From Editor Project
#include "EditorComponent.hpp"
#include "EditorObject.hpp"
//Third Party Library
#include "glm/ext.hpp"
//Standard Library
#include <memory>
#include <vector>
#include <map>

namespace Tools
{
	class PrefabEditor;
	using GameObjectHandle = UNIQUE_HANDLE(GameObject);
	using ComponentHandle = UNIQUE_HANDLE(EditorComponent);
	using TransformHandle = UNIQUE_HANDLE(TransformEC);
	using ComponentsMap = std::map<std::string, EditorComponent*>;
	//using MeshRendererHandle = UNIQUE_HANDLE(MeshRendererEC);
	//using ComponentList = std::vector<std::unique_ptr<EditorComponent>>;

	class EditorEntity : public EditorObject
	{
		friend class PrefabEditor;
		friend class Inspector;
	private:
		std::string* m_objectName;
		int* m_objectID;
		bool* m_isActive;
		GameObjectHandle m_gameObject;
		std::vector<EditorComponent*> m_components;
		ComponentsMap m_componentsMap;

	public:
		EditorEntity();
		EditorEntity(GameObject* gameObject);
		
		//Override Methods
		virtual void OnRender() override;

		//Interface
		void AddComponent(std::string);
		EditorComponent* GetComponent(std::string);
		bool RemoveComponent(std::string);
		std::vector<EditorComponent*>& GetComponentList();
		void PrintInfo();
		std::string GetName();
		int GetID();
	};
}