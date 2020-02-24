#include "Inspector.hpp"
#include "Graphic/GraphicCore.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>
#include <array>

namespace Tools
{
	void Inspector::Init(void)
	{
		ImVec2 main_window_size = ImVec2(Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
		ImVec2 main_window_pos = ImGui::GetMainViewport()->Pos;
		std::cout << "Viewport Pos " << main_window_pos.x << " ," << main_window_pos.y << std::endl;
		m_width = main_window_size.x / 4;
		m_height = main_window_size.y - 20;
		m_position.x = (main_window_size.x - m_width);
		m_position.y = main_window_pos.y + 20;
		
		m_windowFlags |= ImGuiWindowFlags_NoCollapse;
		m_windowFlags |= ImGuiWindowFlags_NoMove;
		m_windowFlags |= ImGuiWindowFlags_NoResize;
	}

	void Inspector::OnRender(void)
	{

		//TODO: Render some header or some shit first
			
		
		if (!m_entityToRender && !m_componentList)
		{
			return;
		}
		ImVec2 window_size = ImGui::GetWindowSize();

		ImGui::PushItemWidth(-1);
		ImGui::Checkbox("##Active", m_entityToRender->m_isActive);
		ImGui::SameLine();
		ImGui::InputText("##EntityName", &*m_entityToRender->m_objectName);
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("Component:");

		//Render Component
		for (auto component : (*m_componentList))
		{
			component->Render();
		}

		//End Component Section
		ImGui::Separator();
		
		//Add Component Button
		ImVec2 button_size = ImVec2(window_size.x / 2, ImGui::GetFontSize() + 10);
		//ImGui::SetCursorPosX(ImGui::GetWindowPos().x);
		ImGui::Indent(button_size.x/2);

		if (ImGui::Button("Add Component", button_size))
		{
			//TODO: Add component to entity
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			ImGui::Selectable("Test Component");
			ImGui::EndPopup();
		}

	}

	void Inspector::SetEntity(EditorEntity * ent)
	{
		m_entityToRender = ent;
		m_componentList = &m_entityToRender->GetComponentList();
	}
}