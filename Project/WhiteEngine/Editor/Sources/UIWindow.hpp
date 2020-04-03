#pragma once
#include <string>
#include <unordered_set>
#include "imgui.h"
namespace Tools
{
	class UIWindow;
	using string = std::string;
	using WindowList = std::unordered_set<UIWindow*>;
	using WindowCondition = ImGuiCond;
	class UIWindow
	{
	private:
		static WindowList m_windowList;
		bool* m_show;
		bool m_isFocused = false;
		bool m_closable;
		string m_windowName;
	protected:
		int m_width, m_height;
		ImVec2 m_position;
		ImGuiWindowFlags m_windowFlags;
		WindowCondition m_windowCondition = ImGuiCond_FirstUseEver;
		virtual void OnRender(void) = 0;
		
		virtual void Init(void) {}
		virtual void Terminate(void) {}
	public:
		UIWindow(string name = "Window", int width = 500, int height = 500, bool* open = NULL, bool closable = true);
		~UIWindow();
		virtual void PreRender(void) {}
		void Render();
		bool GetBool();
		void SetBool(bool show);
		void ToggleBool();
		bool IsClosable();
		void AddWindowFlag(ImGuiWindowFlags flags);
		void RemoveWindowFlag(ImGuiWindowFlags flags);
		void SetWindowCondition(WindowCondition cond);
		string GetName();
		bool isFocused() { return m_isFocused; };
		static WindowList GetWindowList();
	};
	
}
