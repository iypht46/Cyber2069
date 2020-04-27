#include "ObjectHierarchy.hpp"

#include "Graphic/Window.hpp"

namespace Tools
{
	void ObjectHierarchy::Init()
	{
		ImVec2 main_window_size = ImVec2(static_cast<float>(Graphic::Window::GetWidth()), static_cast<float>(Graphic::Window::GetHeight()));
		ImVec2 main_window_pos = ImGui::GetMainViewport()->Pos;
		std::cout << "Viewport Pos " << main_window_pos.x << " ," << main_window_pos.y << std::endl;
		m_width = static_cast<int>(main_window_size.x / 4);
		m_height = static_cast<int>(main_window_size.y - 20);
		m_position.x = 0;
		m_position.y = main_window_pos.y + 20;

		m_windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
	}

	void ObjectHierarchy::OnRender()
	{
		if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && m_objectToRender)
		{
			ImGui::OpenPopup("##HierarychyAction");
		}

		if (ImGui::BeginPopup("##HierarychyAction"))
		{
			if (ImGui::Selectable("Create Empty"))
			{
				m_objectToRender->AddObject();
			}
			ImGui::EndPopup();
		}

		ImVec2 window_size = ImGui::GetWindowSize();

		if (ImGui::BeginMenuBar())
		{
			ImGui::PushItemWidth(-1);
			if (ImGui::BeginMenu("Create"))
			{
				ImGui::EndMenu();
			}

			ImGui::Text("Search: "); ImGui::SameLine();
			m_filter.Draw("##ObjectSearch", m_width / 2);
			ImGui::PopItemWidth();
			ImGui::EndMenuBar();
		}

		
		if (m_objectToRender)
			m_objectToRender->Render();

	}
}
