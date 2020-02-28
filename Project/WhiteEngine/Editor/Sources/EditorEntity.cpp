#include "EditorEntity.hpp"
#include "CoreComponentEC.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"


namespace Tools
{
	EditorEntity::EditorEntity() 
	{
		//Create transform
		m_gameObject = std::make_unique<GameObject>();
		 TransformEC* transform = new TransformEC();
		 transform->Init(&m_gameObject->m_transform);
		 m_components.push_back(transform);
		 //Point to gameobject member
		 m_objectName = &m_gameObject->m_objectName;
		 m_isActive = &m_gameObject->isActive;
	}

	EditorEntity::EditorEntity(GameObject * gameObject)
	{
		//Create gameobject from parameter
		m_gameObject = std::make_unique<GameObject>(*gameObject);
		//Create transform
		TransformHandle transform = make_unique<TransformEC>();
		transform->Init(&m_gameObject->m_transform);
		m_components.push_back(transform.get());
		//Point to gameobject member
		m_objectName = &m_gameObject->m_objectName;
		m_isActive = &m_gameObject->isActive;
	}

	void EditorEntity::AddComponent(std::string type)
	{
		//Check if component have been added
		if (m_componentsMap[type])
		{
			return;
		}

		Component* comp = nullptr;
		if (type.find("MeshRendererEC") != std::string::npos)
		{
			comp = m_gameObject->AddComponent<MeshRenderer>();
		}
		else if (type.find("BoxColliderEC") != std::string::npos)
		{
			comp = m_gameObject->AddComponent<BoxCollider>();
		}

		if (comp)
		{
			EditorComponent* compEC = EditorComponent::makeComponent(type);
			compEC->Init(comp);
			m_components.push_back(compEC);
			m_componentsMap[type] = true;
			//ComponentHandle editorComp = make_unique<EditorComponent>(component);
			//m_components.push_back(std::move(editorComp));
		}
	}

	bool EditorEntity::RemoveComponent(std::string name)
	{
		if (m_componentsMap[name])
		{
			//Retrieve component from vector and erased it
		}

		return false;
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
