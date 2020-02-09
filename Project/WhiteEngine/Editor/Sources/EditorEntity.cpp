#include "EditorEntity.hpp"
#include "Core/EC/GameObject.hpp"

namespace Tools
{
	EditorEntity::EditorEntity() : m_gameObject(std::make_unique<Core::GameObject>().get()) 
	{
	
	}

	void EditorEntity::AddComponent()
	{
		//m_gameObject.get()->
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
