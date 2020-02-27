#include "imgui.h"
#include "EditorComponent.hpp"
#include "Core/Logger.hpp"


namespace Tools
{
	//*****Prototype Design Variable*****//
	

	//*****          END            *****//

	void EditorComponent::Render()
	{
		if (!ImGui::CollapsingHeader(m_componentName.c_str(), &m_open))
		{
			//TODO: Remove Component
			
		}
		else
		{
			ImGui::Text("Enable"); ImGui::SameLine();
			ImGui::Checkbox("##IsEnable", m_enable);
			OnRender();
		}
		
	}

	EditorComponent* EditorComponent::makeComponent(std::string type)
	{
		auto componentEC = m_componentTable.find(type);
		
		if (componentEC != m_componentTable.end())
			return componentEC->second->clone();

		return nullptr;
	}

	EditorComponent* EditorComponent::addComponent(std::string type, EditorComponent* comp)
	{
		//ENGINE_INFO("Adding prototype for {}", type);
		m_componentTable[type] = comp;
		m_availableComponent.push_back(type);

		return comp;
	}

	AvailableComponent* EditorComponent::GetTableList()
	{
		return &m_availableComponent;
	}

	
	
}


