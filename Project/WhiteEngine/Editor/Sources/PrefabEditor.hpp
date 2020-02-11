#pragma once

#include <memory>
#include "Editor.hpp"
#include "ComponentsBrowser.hpp"
#include "EditorEntity.hpp"
#include "PreviewWindow.hpp"

namespace Tools
{
	using EntityHandle = std::unique_ptr<EditorEntity>;

	class PrefabEditor : public Editor
	{
	private:
		//
		//TODO: Add UI Window Elements
		EntityHandle m_editorEntitiy;
		ComponentsBrowser m_componentBrowser;
		//TODO: Preview Window
		PreviewWindow m_previewWindow;
	public:
		PrefabEditor() : Editor(EDITOR_TYPE::PREFAB_EDITOR) {}
		virtual void Init(void) override;
		virtual void Update(void) override;
		virtual void Terminate(void) override;
		virtual bool Save(const char*) override;
		virtual bool Load(const char*) override;
		//Interface
		//TODO: Load from prefab file
		bool LoadPrefab(const char* filename);
		//TODO: Save Prefab file
		bool SavePrefab(const char* filename);

	};
}