#pragma once
#include <memory>

#include "Core/Factory.h"
#include "Core/EC/GameObject.hpp"

namespace Tools
{
	class PrefabEditor;
	using gameObject = std::unique_ptr<GameObject>;

	class EditorEntity
	{
		friend class PrefabEditor;
	private:
		
		std::string m_objectName;
		int m_objectID;
		gameObject m_gameObject;

	public:
		EditorEntity();
		
		//Interface
		void AddComponent();
		std::string GetName();
		int GetID();
	};
}