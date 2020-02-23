#pragma once

#include "Editor.hpp"
#include "EngineCore.hpp"
#include "Inspector.hpp"
#include "SceneEditor.hpp"
#include "PrefabEditor.hpp"

class GameObject;

namespace Tools
{
	using EditorHandle = UNIQUE_HANDLE(Editor);
	using InspectorHandle = UNIQUE_HANDLE(Inspector);

	class MainEditor
	{
	private:
		EditorHandle m_currentEditor;
		InspectorHandle m_inspector;
		//Private functions
		void MainMenuBar();
		bool AccessEditor(EDITOR_TYPE);
		std::string openfilename(const char *, HWND);
	public:
		MainEditor();
		~MainEditor();
		void Init();
		void Update();
		void Terminate();
		void Save();
		void Load(const char*);
	};
}



