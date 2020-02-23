#include "EditorEntity.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"

namespace Tools
{
	EditorEntity::EditorEntity() 
	{
		//Create transform
		m_gameObject = std::make_unique<GameObject>();
		 TransformEC* tranform = new TransformEC(&m_gameObject->m_transform);
		 m_components.push_back(tranform);
		 //Point to gameobject member
		 m_objectName = &m_gameObject->m_objectName;
		 m_isActive = &m_gameObject->isActive;
	}

	EditorEntity::EditorEntity(GameObject * gameObject)
	{
		//Create gameobject from parameter
		m_gameObject = std::make_unique<GameObject>(*gameObject);
		//Create transform
		TransformHandle tranform = make_unique<TransformEC>(&m_gameObject->m_transform);
		m_components.push_back(tranform.get());
		//Point to gameobject member
		m_objectName = &m_gameObject->m_objectName;
		m_isActive = &m_gameObject->isActive;
	}

	void EditorEntity::AddComponent(const char* type)
	{
		Component* component = nullptr;
		if (type == "MeshRenderer")
		{
			component = m_gameObject->AddComponent<MeshRenderer>();
		}

		if (component)
		{
			//ComponentHandle editorComp = make_unique<EditorComponent>(component);
			//m_components.push_back(std::move(editorComp));
		}
		//m_gameObject.get()->

	}

	std::vector<EditorComponent*>& EditorEntity::GetComponentList()
	{
		return m_components;
	}

	std::string EditorEntity::GetName()
	{
		return *m_objectName;
	}

	int EditorEntity::GetID()
	{
		return *m_objectID;
	}

}
