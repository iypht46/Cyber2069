#include "imgui.h"
#include "EditorComponent.hpp"
#include "Core/Logger.hpp"


namespace Tools
{

	EditorComponent::EditorComponent(std::string name, bool canDisable) : m_componentName(name)
	{
		m_id = ++m_totalNum;
		if (canDisable)
			m_enable = new bool(true);
		//m_enable = true;
	}

	bool EditorComponent::Render()
	{
		if (ImGui::CollapsingHeader(m_componentName.c_str(), &m_open))
		{
			if (m_enable)
			{
				ImGui::Text("Enable"); ImGui::SameLine();
				if (ImGui::Checkbox("##IsEnable", m_enable))
					m_component->SetEnable(*m_enable);
			}
			
			OnRender();
		}

		return m_open;
	}

	std::string EditorComponent::GetName()
	{
		return m_componentName;
	}

	int EditorComponent::GetID()
	{
		return m_id;
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


