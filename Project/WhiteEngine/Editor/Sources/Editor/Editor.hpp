#pragma once
//From Engine
#include "Core/Handle.hpp"
#include "Utility/Filesystem.hpp"
#include "Container/String.hpp"
//From Editor
#include "EditorObject/EditorEntity.hpp"
#include "EditorObject/EditorObject.hpp"
#include "EditorWindow/UIWindow.hpp"
#include "EditorWindow/PopupWindow.hpp"

//Third Party Library
#include <imfilebrowser.h>
//Standard Library

namespace Tools
{

	
	using Path = Utility::fs::path;
	using PathHandle = UNIQUE_HANDLE(Path);
	enum class EDITOR_ACTION : unsigned int { EDITOR_NONE = 0, EDITOR_SAVE, EDITOR_LOAD};

	class Editor
	{
	protected:
		Container::wString m_editorName = "Default";
		bool m_dirtyFlag = false;
		bool* m_open;
		EditorEntity* m_selectedEntity = nullptr;
		UIWindow* m_mainWindow;
		PathHandle m_path;
		ImGui::FileBrowser* m_fileDialog;
		EDITOR_ACTION m_action;
		PopupWindow m_popup;
	protected:
		virtual void Init(void) = 0;
		virtual void OnUpdate(void) = 0;
		virtual void Terminate(void) = 0;
	public:
		static Utility::fs::path s_editorBasePath;
		Editor(Container::wString editor_name, bool* isOpen = new bool(false));
		Container::wString GetName();
		bool HasSaved();

		virtual void Update(void);
		virtual bool Save(Utility::fs::path path, Container::wString& returnMessage) = 0;
		virtual bool Load(Utility::fs::path path, Container::wString& returnMessage) = 0;
		virtual bool New() = 0;
		virtual EditorEntity* GetSelectedEntity();
		virtual EditorObject* GetEditorObject();
		virtual bool IsFocused();
		virtual bool IsOpen();
		virtual bool IsDirty();
		virtual void ToggleOpen();
		virtual void SetOpen(bool open);
		virtual void SetDirty(bool dirty);
		virtual void RenderMenu() {}
	};
	
}
