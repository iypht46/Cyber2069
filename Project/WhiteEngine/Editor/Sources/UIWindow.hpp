#pragma once
#include <string>
#include "imgui.h"
namespace Tools
{
	class UIWindow
	{
	private:
		bool* m_show;
		std::string m_windowName;
	protected:
		int m_width, m_height;
		ImVec2 m_position;
		ImGuiWindowFlags m_windowFlags;
		virtual void OnRender(void) = 0;
		virtual void Init(void) {}
		virtual void Terminate(void) {}
	public:
		UIWindow(std::string name = "Window", int width = 500, int height = 500, bool* open = NULL);
		~UIWindow();
		void Render();
		bool GetBool();
		void ToggleBool();
		bool IsFocused();
	};
	
}
