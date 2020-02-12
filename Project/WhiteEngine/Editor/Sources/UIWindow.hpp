#pragma once
#include <string>
#include "imgui.h"
namespace Tools
{
	class UIWindow
	{
	private:
		bool m_show = true;
		std::string m_windowName;
	protected:
		virtual void OnRender(void) = 0;
		virtual void Init(void) = 0;
		virtual void Terminate(void) = 0;
	public:
		UIWindow(std::string);
		~UIWindow();
		void Render();
		bool GetBool();
		void ToggleBool();
	};

	inline void UIWindow::Render()
	{
		ImGui::Begin(m_windowName.c_str(), &m_show);
		OnRender();
		ImGui::End();
	}
	inline UIWindow::UIWindow(std::string name) : m_windowName(name) { m_show = true; Init(); }
	inline bool UIWindow::GetBool(void) { return m_show; }
	inline void UIWindow::ToggleBool() { m_show = !m_show; }
	inline UIWindow::~UIWindow() { Terminate(); }
}
