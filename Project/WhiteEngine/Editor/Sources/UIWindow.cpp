#include "UIWindow.hpp"

namespace Tools
{
	void UIWindow::Render()
	{
		//Window size and position can be set in init method of inheriting class.
		ImGui::SetNextWindowPos(m_position);
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
		//Window flag should also be set in init method
		ImGui::Begin(m_windowName.c_str(), m_show, m_windowFlags);
		OnRender();
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