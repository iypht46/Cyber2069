#pragma once

#include <memory>
#include "Editor.hpp"
#include "Inspector.hpp"
#include "EditorEntity.hpp"
#include "PreviewWindow.hpp"
#include "EngineCore.hpp"

namespace Tools
{
	using EntityHandle = UNIQUE_HANDLE(EditorEntity);
	using PreviewHandle = UNIQUE_HANDLE(PreviewWindow);

	class PrefabEditor : public Editor
	{
	private:
		//TODO: Add UI Window Elements
		EntityHandle m_editorEntitiy;
		//TODO: Preview Window
		PreviewHandle m_previewWindow;
		bool m_previewBool = true;
	protected:
		virtual void Init(void) override;
		virtual void Terminate(void) override;
		//TODO: Load from prefab file
		bool LoadPrefab(const char* filename);
		//TODO: Save Prefab file
		bool SavePrefab(const char* filename);
	public:
		PrefabEditor(bool* isOpen);
		PrefabEditor();
		//Interface
		virtual void Update(void) override;
		virtual bool Save(const char* path, Container::wString& returnMessage) override;
		virtual bool Load(const char* path, Container::wString& returnMessage) override;
		virtual bool IsFocused() override;
		virtual void RenderMenu() override;
		virtual EditorObject* GetEditorObject() override;
	};
}