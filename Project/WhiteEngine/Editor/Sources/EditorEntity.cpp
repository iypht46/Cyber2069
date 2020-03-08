#include "EditorEntity.hpp"
#include "CoreComponentEC.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"

#include <algorithm>
#include <iostream>

namespace Tools
{
	EditorEntity::EditorEntity() 
	{
		//Create transform
		m_gameObject = std::make_unique<GameObject>();
		 TransformEC* transform = new TransformEC();
		 transform->Init(&m_gameObject->m_transform);
		 m_componentsMap[transform->GetName()] = transform; //Add to map of component name and component*
		 m_components.push_back(transform); //Add to vector of component*
		 //Point to gameobject member
		 m_objectName = &m_gameObject->m_objectName;
		 m_isActive = &m_gameObject->isActive;
	}

	EditorEntity::EditorEntity(GameObject * gameObject)
	{
		//Create gameobject from parameter
		m_gameObject = std::make_unique<GameObject>(*gameObject);
		//Create transform
		TransformEC* transform = new TransformEC();
		transform->Init(&m_gameObject->m_transform);
		m_componentsMap[transform->GetName()] = transform; //Add to map of component name and component*
		m_components.push_back(transform); //Add to vector of component*
		//m_components.push_back(transform.get());
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
		else if (type.find("RigidbodyEC") != std::string::npos)
		{
			comp = m_gameObject->AddComponent<Rigidbody>();
		}

		if (comp)
		{
			EditorComponent* compEC = EditorComponent::makeComponent(type);
			compEC->Init(comp);
			m_componentsMap[compEC->GetName()] = compEC;
			m_components.push_back(compEC);
			//ComponentHandle editorComp = make_unique<EditorComponent>(component);
			//m_components.push_back(std::move(editorComp));
		}
	}

	EditorComponent * EditorEntity::GetComponent(std::string name)
	{
		if (m_componentsMap.find(name) == m_componentsMap.end())
			return nullptr;

		return m_componentsMap[name];
	}

	bool EditorEntity::RemoveComponent(std::string name)
	{
		if (m_componentsMap.find(name) == m_componentsMap.end())
			return false;

		m_components.erase(std::remove(m_components.begin(), m_components.end(), m_componentsMap[name]), m_components.end());
		m_componentsMap.erase(name);
		return true;
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
