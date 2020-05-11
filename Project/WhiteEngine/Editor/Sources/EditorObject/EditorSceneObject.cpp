#include "EditorSceneObject.hpp"
#include "Serialization/Serialization.h"

namespace Tools
{
	EditorSceneObject::EditorSceneObject()
	{
		m_sceneObject = std::make_unique<SceneManagement::Scene>();
	}

	EditorSceneObject::EditorSceneObject(SceneManagement::Scene* obj)
	{
		m_sceneObject = std::make_unique<SceneManagement::Scene>(*obj);

		for (auto gameObj : m_sceneObject->GameObjectsInScene)
		{
			EditorEntity* ent = new EditorEntity(gameObj.second);
			m_entityInScene.insert(ent);
		}
	}

	SceneManagement::Scene* EditorSceneObject::GetSceneObject()
	{
		return m_sceneObject.get();
	}

	void EditorSceneObject::AddObject()
	{
		this->AddEntity();
	}

	void EditorSceneObject::AddObject(EditorObject* obj)
	{
		EditorEntity* ent = dynamic_cast<EditorEntity*>(obj);

		if (ent)
			this->AddEntity(ent);
		else
			ENGINE_ERROR("ERROR: Object is not entity!");
	}

	void EditorSceneObject::AddObjToScene(GameObject* obj)
	{
		std::shared_ptr<GameObject> ptr = std::make_shared<GameObject>(*obj);
		m_sceneObject->GameObjectsInScene[ptr->GetID()] = ptr;
	}

	EditorEntity* EditorSceneObject::AddEntity()
	{
		EditorEntity* ent = new EditorEntity(); //Wrap with entity class
		this->AddEntity(ent);
		return ent;
	}

	void EditorSceneObject::AddEntity(EditorEntity* ent)
	{
		ent->SetScene(this);
		m_sceneObject->GameObjectsInScene[ent->GetID()] = ent->GetGameObjectHandle();
		m_entityInScene.insert(ent);
	}

	void EditorSceneObject::RemoveEntity(EditorEntity* ent)
	{
		m_removeQueue.push(ent);
	}

	void EditorSceneObject::AddPrefab(Utility::fs::path prefabPath)
	{
		if (Utility::fs::exists(prefabPath))
		{
			//LoadObject
		}
	}

	bool EditorSceneObject::Load(Container::wString path)
	{
		if (m_sceneObject)
		{
			m_sceneObject.reset();
			m_sceneObject = std::make_unique<SceneManagement::Scene>();
		}
		m_entityInScene.clear();

		Serialization::LoadObject(*m_sceneObject.get(), path);

		if (m_sceneObject.get())
		{
			for (auto gameObj : m_sceneObject->GameObjectsInScene)
			{
				//If gameobject is not a child object
				if (!gameObj.second->m_transform->HasParent())
				{
					EditorEntity* ent = new EditorEntity(gameObj.second); //Add to scene to be render to hierarchy
					ent->SetScene(this);
					m_entityInScene.insert(ent);
				}
				else
				{
					gameObj.second->SetAllComponents();
				}
			}

			for (auto entity : m_entityInScene)
			{
				entity->LoadChildObject();
			}


		}

		return (m_sceneObject.get());
	}

	bool EditorSceneObject::Save(Container::wString path)
	{
		Serialization::SaveObject(*m_sceneObject, path);

		return Utility::fs::is_regular_file(Utility::fs::path(path));
	}

	EditorSceneObject::~EditorSceneObject()
	{
	}

	void EditorSceneObject::PreRender()
	{
		m_open = ImGui::TreeNode("Scene");
	}

	void EditorSceneObject::OnRender()
	{
		if (!m_open)
			return;

		for (auto ent : m_entityInScene)
		{
			ent->Render();
		}

		ProcessRemoveQueue();

		if (m_entityInScene.size() == 0)
		{
			m_selected = nullptr;
		}
		else
		{
			for (auto ent : m_entityInScene)
			{

				if (ent->GetSelected())
					m_selected = ent->GetSelected();
			}
		}
		
	}

	void EditorSceneObject::PostRender()
	{
		if (m_open)
			ImGui::TreePop();
	}

	void EditorSceneObject::ProcessRemoveQueue()
	{
		if (m_removeQueue.size() > 0)
		{
			auto ent = m_removeQueue.front();
			m_removeQueue.pop();

			auto it = m_entityInScene.find(ent);

			if (it != m_entityInScene.end())
			{
				//Remove Entity from set
				auto entity = *it;
				m_entityInScene.erase(it);

				//Removing Gameobject from scene object
				auto gameObjectMap = m_sceneObject->GameObjectsInScene;
				auto gameObjectIT = gameObjectMap.find(entity->GetGameObject()->GetID());
				
				if (gameObjectIT != gameObjectMap.end())
				{
					auto gameObject = (*gameObjectIT).second;
					gameObjectMap.erase(gameObjectIT);
					gameObject.reset();
				}

				delete entity;
			}
			m_selected = nullptr;
		}
	}
}
