#pragma once
//Editor
#include "Editor/Editor.hpp"
#include "EditorWindow/Inspector.hpp"
#include "EditorObject/EditorEntity.hpp"
#include "EditorWindow/PreviewWindow.hpp"
#include "EngineCore.hpp"
//Standard Library
#include <memory>




namespace Tools
{
	using EntityHandle = UNIQUE_HANDLE(EditorEntity);
	using PreviewHandle = UNIQUE_HANDLE(PreviewWindow);

	class PrefabEditor : public Editor
	{
	private:
		//TODO: Add UI Window Elements
		EntityHandle m_entityObject;
		//TODO: Preview Window
		PreviewHandle m_previewWindow;
		bool m_previewBool = true;
		//Popup Window
		PopupWindow m_prefabPopup;
	protected:
		virtual void Init(void) override;
		virtual void OnUpdate(void) override;
		virtual void Terminate(void) override;
	public:
		PrefabEditor(bool* isOpen);
		PrefabEditor();
		//Interface
		virtual bool Save(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool Load(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool New();
		virtual bool IsFocused() override;
		virtual void RenderMenu() override;
		virtual EditorObject* GetEditorObject() override;
	};
}