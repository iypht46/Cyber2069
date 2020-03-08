#pragma once

#include "Editor.hpp"
#include "EngineCore.hpp"
#include "Inspector.hpp"
#include "SceneEditor.hpp"
#include "PrefabEditor.hpp"

class GameObject;

namespace Tools
{
	using PrefabEditorHandle = UNIQUE_HANDLE(PrefabEditor);
	using SceneEditorHandle = UNIQUE_HANDLE(SceneEditor);
	using InspectorHandle = UNIQUE_HANDLE(Inspector);

	class MainEditor
	{
	private:
		Editor* m_currentEditor;
		PrefabEditorHandle m_prefabEditor;
		bool m_prefabBool = false;
		SceneEditorHandle m_sceneEditor;
		bool m_sceneBool = false;
		InspectorHandle m_inspector;
		bool m_inspectorBool = true;
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
		void Load(Editor*, const char*);
	};
}



