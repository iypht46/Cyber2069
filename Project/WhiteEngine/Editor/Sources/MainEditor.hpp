#pragma once

#include <map> //For pair

#include "Editor.hpp"
#include "EngineCore.hpp"
#include "Inspector.hpp"
#include "ObjectHierarchy.hpp"
#include "SceneEditor.hpp"
#include "PrefabEditor.hpp"

class GameObject;

namespace Tools
{
	enum class EDITOR_TYPE { PREFAB_EDITOR, SCENE_EDITOR };

	using EditorHandle = UNIQUE_HANDLE(Editor);
	using PrefabEditorHandle = UNIQUE_HANDLE(PrefabEditor);
	using SceneEditorHandle = UNIQUE_HANDLE(SceneEditor);
	using InspectorHandle = UNIQUE_HANDLE(Inspector);
	using HierarchyHandle = UNIQUE_HANDLE(ObjectHierarchy);
	using EditorHandle = UNIQUE_HANDLE(Editor);
	using EditorMap = std::unordered_map<EDITOR_TYPE, EditorHandle>;
	class MainEditor
	{
	private:
		//Editor List
		EditorMap m_editorMap;
		PrefabEditorHandle m_prefabEditor;
		SceneEditorHandle m_sceneEditor;
		Editor* m_currentEditor;
		//Window List
		InspectorHandle m_inspector;
		HierarchyHandle m_hierarchy;

		//Private functions
		void MainMenuBar();
		bool AccessEditor(Editor* editor);
		bool AccessEditor(EDITOR_TYPE type);
		std::string openfilename(const char *filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);
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



