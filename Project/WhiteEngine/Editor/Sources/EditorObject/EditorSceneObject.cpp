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
			EditorEntity* ent = new EditorEntity(gameObj);
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
		m_sceneObject->GameObjectsInScene.insert(ptr);
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
		//std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(*ent->GetGameObject());
		m_sceneObject->GameObjectsInScene.insert(ent->GetGameObjectHandle());
		m_entityInScene.insert(ent);
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
				if (!gameObj->m_transform->HasParent())
				{
					EditorEntity* ent = new EditorEntity(gameObj); //Add to scene to be render to hierarchy
					ent->SetScene(this);
					m_entityInScene.insert(ent);
				}
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

			if (ent->GetSelected())
				m_selected = ent->GetSelected();
		}
	}

	void EditorSceneObject::PostRender()
	{
		if (m_open)
			ImGui::TreePop();
	}
}
