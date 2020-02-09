#pragma once
#include <memory>

#include "Core/Factory.h"

namespace Core
{
	class GameObject;
}

namespace Tools
{
	using GameObject = std::unique_ptr<Core::GameObject>;

	class EditorEntity
	{
	private:
		std::string m_objectName;
		int m_objectID;
		const GameObject m_gameObject;

	public:
		EditorEntity();
		
		//Interface
		void AddComponent();
		std::string GetName();
		int GetID();
	};
}