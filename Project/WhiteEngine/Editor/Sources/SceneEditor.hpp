#pragma once

#include "Editor.hpp"
#include "Inspector.hpp"
#include "GameplayWindow.hpp"
#include "EditorSceneObject.hpp"

#include <utility>
#include <map>
#include <memory>

namespace Tools
{
	using SceneHandle = UNIQUE_HANDLE(EditorSceneObject);
	using GameplayHandle = UNIQUE_HANDLE(GameplayWindow);
	//using EntityMap = std::map<int, EntityHandle>;

	class SceneEditor : public Editor
	{
	private:
		static EditorEntity *m_selectedEntity;
		SceneHandle m_sceneObject;
		//TODO: Add Gameplay Window
		GameplayHandle m_gameplayWindow;
		//TODO: Add Scene Window

		//TODO: Add Scene Wrapper for editor

	protected:
		virtual void Init(void) override;
		virtual void Terminate(void) override;
		bool LoadScene(const char*);
		bool SaveScene(const char*);
	public:
		SceneEditor(bool* isOpen);
		SceneEditor();
		//Interface
		virtual void Update(void) override;
		virtual bool Save(const char* path, Container::wString& returnMessage) override;
		virtual bool Load(const char* path, Container::wString& returnMessage) override;
		virtual bool IsFocused() override;
		virtual void RenderMenu() override;
		virtual EditorObject* GetEditorObject() override;
		
	};
}
