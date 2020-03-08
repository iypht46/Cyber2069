#include "GameObjectHierarchy.hpp"
#include "Graphic/GraphicCore.hpp"

namespace Tools
{

	void GameObjectHierarchy::Init()
	{
		ImVec2 main_window_size = ImVec2(Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
		ImVec2 main_window_pos = ImGui::GetMainViewport()->Pos;
		//std::cout << "Viewport Pos " << main_window_pos.x << " ," << main_window_pos.y << std::endl;
		m_width = main_window_size.x / 4;
		m_height = main_window_size.y - 20;
		m_position.x = (m_width);
		m_position.y = main_window_pos.y + 20;

		m_windowFlags |= ImGuiWindowFlags_NoResize;
	}

	void GameObjectHierarchy::OnRender()
	{

	}


}