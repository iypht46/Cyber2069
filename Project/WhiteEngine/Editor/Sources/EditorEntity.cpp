#include "EditorEntity.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"

namespace Tools
{
	EditorEntity::EditorEntity() : m_gameObject(std::make_unique<GameObject>().get()) 
	{
		this->AddComponent("Transform");
		this->AddComponent("MeshRenderer");
	}

	void EditorEntity::AddComponent(const char* type)
	{
		Component* component = nullptr;
		if (type == "Transform")
		{
			component = m_gameObject->AddComponent<Transform>();
		}
		else if (type == "MeshRenderer")
		{
			component = m_gameObject->AddComponent<MeshRenderer>();
		}

		if (component)
		{
			ComponentHandle editorComp = make_unique<EditorComponent>(component);
			m_components.push_back(editorComp);
		}
		//m_gameObject.get()->

	}

	ComponentList& EditorEntity::GetComponentList()
	{
		return m_components;
	}

	std::string EditorEntity::GetName()
	{
		return m_objectName;
	}

	int EditorEntity::GetID()
	{
		return m_objectID;
	}

}
