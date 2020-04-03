#pragma once
//From Engine
#include "Core/Handle.hpp"
#include "Utility/Filesystem.hpp"
#include "Container/String.hpp"
//From Editor
#include "EditorEntity.hpp"
#include "UIWindow.hpp"
#include "EditorObject.hpp"
//Third Party Library
#include "imfilebrowser.h"
//Standard Library

namespace Tools
{

	
	using Path = Utility::fs::path;
	using PathHandle = UNIQUE_HANDLE(Path);

	class Editor
	{
	protected:
		Container::wString m_editorName = "Default";
		bool m_dirtyFlag = false;
		bool* m_open;
		EditorEntity* m_selectedEntity;
		UIWindow* m_mainWindow;
		PathHandle m_path;
		ImGui::FileBrowser m_fileDialog;
	public:
		static Utility::fs::path s_editorBasePath;
		Editor(Container::wString editor_name, bool* isOpen = new bool(false));
		Container::wString GetName();
		bool HasSaved();

		virtual void Init(void) = 0;
		virtual void Update(void) = 0;
		virtual void Terminate(void) = 0;
		virtual bool Save(const char* path, Container::wString& returnMessage) = 0;
		virtual bool Load(const char* path, Container::wString& returnMessage) = 0;
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
