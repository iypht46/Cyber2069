#pragma once
//White Engine
#include "Core/Factory.h"
#include "Core/EC/GameObject.hpp"
#include "Core/Handle.hpp"
#include "Core/Logger.hpp"
//From Editor Project
#include "EditorComponent.hpp"
#include "EditorObject.hpp"
//Third Party Library
#include "imgui.h"
#include "glm/ext.hpp"
//Standard Library
#include <memory>
#include <vector>
#include <map>

namespace Tools
{
	//Forward Declarations
	class EditorSceneObject;
	class PrefabEditor;
	class TransformEC;
	//Using Directives
	using GameObjectHandle = SHARED_HANDLE(GameObject);
	using ComponentHandle = UNIQUE_HANDLE(EditorComponent);
	using TransformHandle = UNIQUE_HANDLE(TransformEC);
	using ComponentsEC = std::vector<EditorComponent*>;
	using ChildsEntityEC = std::vector<EditorEntity*>;
	using ComponentsMap = std::map<std::string, EditorComponent*>;
	//using MeshRendererHandle = UNIQUE_HANDLE(MeshRendererEC);
	//using ComponentList = std::vector<std::unique_ptr<EditorComponent>>;

	class EditorEntity : public EditorObject
	{
		friend class PrefabEditor;
		friend class Inspector;
	private:
		std::string m_objectName;
		bool m_isActive;
		GameObjectHandle m_gameObject; //Gameobject unique pointer
		TransformHandle m_transformEC; //Transform unique pointer
		ChildsEntityEC m_childEntities; //Vector of Editor Entity of child object
		EditorSceneObject* m_scene; //Scene this object is in
		ComponentsEC m_components;
		ComponentsMap m_componentsMap;
		ImGuiTreeNodeFlags m_nodeFlag = ImGuiTreeNodeFlags_OpenOnArrow;
		void AddComponentToEntity(EditorComponent* compEC);
	protected:
		//Override Methods
		virtual void PreRender() override;
		virtual void OnRender() override;
		virtual void PostRender() override;
		void LoadChildObject(); //Wrap child object with EditorEntity object and put into m_childEntities container.
		void LoadComponents(); //Wrap components with EditorComponent object and put into m_components and m_componentsMap container.
		void LoadGameObject(); //Load all gameobject data into entity (Child Object & Components).
	public:
		//Constructor
		EditorEntity();
		EditorEntity(GameObjectHandle gameObject);
		EditorEntity(GameObject* obj);

		//Editor Object Interface
		virtual void AddObject(GameObjectHandle obj);
		virtual void AddObject(EditorObject* obj) override;
		virtual void AddObject() override;
		virtual bool Load(Container::wString path) override;
		virtual bool Save(Container::wString path) override;

		//Entity Interface
		void RenderGameObject(glm::mat4 cam);
		void SetScene(EditorSceneObject* scene);
		bool IsInScene();
		void SetParent(EditorEntity* ent);
		GameObject* GetGameObject();
		GameObjectHandle GetGameObjectHandle();
		TransformEC* GetTransformEC();
		void AddComponent(std::string); //Add component by name, use in inspector
		void AddComponent(Component* comp); //Add raw component object, use in loading gameobject from file
		EditorComponent* GetComponent(std::string);
		bool RemoveComponent(std::string);
		std::vector<EditorComponent*>& GetComponentList();
		void PrintInfo();
		std::string GetName();
		int GetID();
	};
}