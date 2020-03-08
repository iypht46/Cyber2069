#include "UIWindow.hpp"

namespace Tools
{
	void UIWindow::Render()
	{
		//Window size and position can be set in init method of inheriting class.
		//ImGui::SetNextWindowPos(m_position);
		PreRender();
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);
		//Window flag should also be set in init method
		ImGui::Begin(m_windowName.c_str(), m_show, m_windowFlags);
		/*ImVec2 newSize = ImGui::GetWindowSize();
		m_width = newSize.x;
		m_height = newSize.y;*/
		OnRender();
		m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		ImGui::End();
	}
	
	UIWindow::UIWindow(std::string name,int width, int height, bool* open)
		: m_windowName(name), m_width(width), m_height(height) 
	{
		m_windowName = name;
		m_show = open;
		m_width = width;
		m_height = height;
	}

	bool UIWindow::GetBool(void) 
	{ 
		return m_show; 
	}

	void UIWindow::ToggleBool() 
	{ 
		*m_show = !m_show;
	}

	UIWindow::~UIWindow() 
	{ 
		this->Terminate(); 
	}
}