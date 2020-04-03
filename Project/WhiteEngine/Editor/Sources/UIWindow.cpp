#include "UIWindow.hpp"

namespace Tools
{
	WindowList UIWindow::m_windowList;

	void UIWindow::Render()
	{
		//Window size and position can be set in init method of inheriting class.
		//ImGui::SetNextWindowPos(m_position);
		PreRender();
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height), m_windowCondition);
		//Window flag should also be set in init method
		ImGui::Begin(m_windowName.c_str(), m_show, m_windowFlags);
		/*ImVec2 newSize = ImGui::GetWindowSize();
		m_width = newSize.x;
		m_height = newSize.y;*/
		OnRender();
		m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		ImGui::End();
	}
	
	UIWindow::UIWindow(string name ,int width, int height, bool* open, bool closable)
		: m_windowName(name), m_width(width), m_height(height) 
	{
		m_show = open;
		m_closable = closable;
		m_windowList.insert(this);
	}

	bool UIWindow::GetBool(void) 
	{ 
		return (m_show) ? *m_show : false;
	}

	void UIWindow::SetBool(bool show)
	{
		if (m_show)
		{
			(*m_show) = show;
		}
	}

	void UIWindow::ToggleBool() 
	{ 
		(*m_show) = !(*m_show);
	}

	bool UIWindow::IsClosable()
	{
		return m_closable;
	}

	void UIWindow::AddWindowFlag(ImGuiWindowFlags flags)
	{
		m_windowFlags |= flags;
	}

	void UIWindow::RemoveWindowFlag(ImGuiWindowFlags flags)
	{
		m_windowFlags &= flags;
	}

	void UIWindow::SetWindowCondition(WindowCondition cond)
	{
		m_windowCondition = cond;
	}

	string UIWindow::GetName()
	{
		return m_windowName;
	}

	WindowList UIWindow::GetWindowList()
	{
		return m_windowList;
	}

	UIWindow::~UIWindow() 
	{ 
		this->Terminate(); 
	}
}