#include "Inspector.hpp"
#include "Graphic/GraphicCore.hpp"
#include "EditorObject/CoreComponentEC.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>
#include <array>

namespace Tools
{
	void Inspector::Init(void)
	{
		ImVec2 main_window_size = ImVec2(Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
		ImVec2 main_window_pos = ImGui::GetMainViewport()->Pos;
		//std::cout << "Viewport Pos " << main_window_pos.x << " ," << main_window_pos.y << std::endl;
		m_width = main_window_size.x / 4;
		m_height = main_window_size.y - 20;
		m_position.x = (main_window_size.x - m_width);
		m_position.y = main_window_pos.y + 20;
		
		//m_windowFlags |= ImGuiWindowFlags_NoCollapse;
		m_windowFlags |= ImGuiWindowFlags_NoResize;
		m_editorComponentList = EditorComponent::GetTableList();
	}

	void Inspector::OnRender(void)
	{
		if (!m_entityToRender)
			return;

		ImVec2 window_size = ImGui::GetWindowSize();

		GameObject* entity = m_entityToRender->GetGameObject();//m_gameObject.get();

		ImGui::PushItemWidth(-1);
		if (ImGui::Checkbox("##Active", &m_entityToRender->m_isActive))
			entity->SetActive(m_entityToRender->m_isActive);
		ImGui::SameLine();
		if (ImGui::InputText("##EntityName", &m_entityToRender->m_objectName))
			entity->SetName(m_entityToRender->m_objectName);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Layer"); ImGui::SameLine();
		ImGui::InputText("##EntityLayer", &entity->Layer);
			
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("Component:");

		//Editor dirty flag. TODO: Send it to editor
		bool dirty_flag = false;

		auto transformEC = m_entityToRender->GetTransformEC();
		if (transformEC)
			transformEC->Render();
		//Render Component
		for (auto component : m_entityToRender->GetComponentList())
		{
			bool close = component->Render();
			if (!close)
			{
				//TODO: Test Remove Component
				m_entityToRender->RemoveComponent(component->GetName());
				continue;
			}

			if (component->dirty)
			{
				dirty_flag = true;
				component->dirty = false;
			}
		}

		//End Component Section
		ImGui::Separator();
		
		//Add Component Button
		ImVec2 button_size = ImVec2(window_size.x / 2, ImGui::GetFontSize() + 10);
		ImGui::Indent(button_size.x/2);
		if (ImGui::Button("Add Component", button_size))
			ImGui::OpenPopup("AddComponent");

		//Add Component Popup
		if (ImGui::BeginPopup("AddComponent"))
		{
			filter.Draw("Search", m_width/2);
			for (auto comp : *m_editorComponentList)
			{
				if (filter.PassFilter(comp.c_str()))
				{
					if (ImGui::Selectable(comp.c_str()))
					{
						m_entityToRender->AddComponent(comp);
						//std::cout << "Add Component: " << comp << std::endl;
					}
				}
					
			}
			ImGui::EndPopup();
		}

	}

	void Inspector::SetEditorObject(EditorObject * obj)
	{
		if (!obj)
		{
			m_entityToRender = nullptr;
			return;
		}
			

		EditorEntity* objEntity = dynamic_cast<EditorEntity*>(obj);

		if (objEntity)
		{
			m_entityToRender = objEntity;
		}
	}

	void Inspector::ResetEditorObject()
	{
		m_entityToRender = nullptr;
	}
}