#pragma once

#include "imgui.h"
#include "imfilebrowser.h"

#include "Editor/Editor.hpp"
#include "EditorWindow/Inspector.hpp"
#include "EditorWindow/GameplayWindow.hpp"
#include "EditorWindow/SceneWindow.hpp"
#include "EditorObject/EditorSceneObject.hpp"
#include "EditorWindow/PopupWindow.hpp"

#include <utility>
#include <map>
#include <memory>

namespace Tools
{
	using SceneHandle = UNIQUE_HANDLE(EditorSceneObject);
	using GameplayHandle = UNIQUE_HANDLE(GameplayWindow);
	using SceneWindowHandle = UNIQUE_HANDLE(SceneWindow);
	using PopupHandle = UNIQUE_HANDLE(PopupWindow);
	//using EntityMap = std::map<int, EntityHandle>;

	class SceneEditor : public Editor
	{
	private:
		
		//Gameplay Window
		GameplayHandle m_gameplayWindow;
		//Scene Window
		bool m_sceneBool;
		SceneWindowHandle m_sceneWindow;
		//Scene Wrapper for editor
		SceneHandle m_sceneObject;
		//Popup Window
		PopupWindow m_scenePopup;
		
	protected:
		virtual void Init(void) override;
		virtual void OnUpdate(void) override;
		virtual void Terminate(void) override;
		bool LoadScene(std::string);
		bool SaveScene(std::string);
	public:
		SceneEditor(bool* isOpen);
		SceneEditor();
		//Interface
		
		virtual bool Save(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool Load(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool New() override;
		virtual bool IsFocused() override;
		virtual void RenderMenu() override;
		virtual EditorObject* GetEditorObject() override;
		
	};
}
