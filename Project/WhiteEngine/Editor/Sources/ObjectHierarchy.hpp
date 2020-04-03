#pragma once
//From Editor
#include "UIWindow.hpp"
#include "EditorObjectRenderer.hpp"
//Third Party Library
#include "imgui.h"
//Standard Library

namespace Tools
{
	class ObjectHierarchy : public UIWindow, public EditorObjectRenderer
	{
	private:
		ImGuiTextFilter m_filter;
	public:
		ObjectHierarchy() : UIWindow("Gameobject Hierarchy", 500, 500, new bool(true), true) { this->Init(); }
		void Init() override;
		void OnRender() override;
	};
}


