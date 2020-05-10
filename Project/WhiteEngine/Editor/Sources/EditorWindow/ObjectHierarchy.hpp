#pragma once
//From Editor
#include "EditorWindow/UIWindow.hpp"
#include "EditorObject/EditorObjectRenderer.hpp"
//Third Party Library
#include "imgui.h"
//Standard Library

namespace Tools
{
	enum class OBJ_TYPE { PREFAB_OBJ, SCENE_OBJ };

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


