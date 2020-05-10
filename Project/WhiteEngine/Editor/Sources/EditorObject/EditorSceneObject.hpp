#pragma once
//White Engine
#include "Core/Handle.hpp"
#include "SceneManagement/SceneManager.h"
#include "Core/Handle.hpp"
#include "Utility/Filesystem.hpp"
//From Editor
#include "EditorObject.hpp"
#include "EditorObject/EditorEntity.hpp"
//Third Party Library

//Standard Library
#include <utility>
#include <queue>

namespace Tools
{
	using SceneObjectHandle = UNIQUE_HANDLE(SceneManagement::Scene);
	//using EntityHandle = SHARED_HANDLE(EditorEntity);

	class EditorSceneObject : public EditorObject
	{
	private:
		SceneObjectHandle m_sceneObject;
		std::set<EditorEntity*> m_entityInScene;
		std::queue<EditorEntity*> m_removeQueue;
	protected:
		virtual void PreRender() override;
		virtual void OnRender() override;
		virtual void PostRender() override;
	public:
		EditorSceneObject();
		EditorSceneObject(SceneManagement::Scene* obj);
		SceneManagement::Scene* GetSceneObject();
		virtual void AddObject() override;
		virtual void AddObject(EditorObject* obj) override;
		void AddObjToScene(GameObject* obj); //Add gameobject to scene object only
		EditorEntity* AddEntity();
		void AddEntity(EditorEntity* ent);
		void RemoveEntity(EditorEntity* ent);
		void ProcessRemoveQueue();
		void AddPrefab(Utility::fs::path prefabPath);
		virtual bool Load(Container::wString path) override;
		virtual bool Save(Container::wString path) override;
		~EditorSceneObject();
		
	};
}


