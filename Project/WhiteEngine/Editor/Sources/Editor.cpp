#include "Editor.hpp"

namespace Tools
{
	Utility::fs::path Editor::s_editorBasePath = Utility::fs::current_path();

	Editor::Editor(Container::wString editor_name, bool* isOpen) : m_editorName(editor_name), m_open(isOpen)
	{

	}

	Container::wString Editor::GetName()
	{
		return m_editorName;
	}

	bool Editor::HasSaved() { return m_dirtyFlag; }

	EditorEntity* Editor::GetSelectedEntity() { return m_selectedEntity; }

	EditorObject * Editor::GetEditorObject()
	{
		return nullptr;
	}

	bool Editor::IsFocused() { return false; }

	bool Editor::IsOpen() 
	{
		return *m_open; 
	}

	bool Editor::IsDirty() 
	{
		return m_dirtyFlag; 
	}

	void Editor::ToggleOpen() 
	{
		*m_open = !(*m_open); 
	}

	void Editor::SetOpen(bool open) 
	{ 
		*m_open = open; 
	}

	void Editor::SetDirty(bool dirty)
	{
		//If setting to dirty
		if (m_dirtyFlag == dirty)
		{
			return;
		}
		else
		{
			if (m_mainWindow)
			{
				if (m_dirtyFlag)
				{
					m_mainWindow->AddWindowFlag(ImGuiWindowFlags_UnsavedDocument);
				}
				else
				{
					m_mainWindow->RemoveWindowFlag(ImGuiWindowFlags_UnsavedDocument);
				}
			}

			m_dirtyFlag = dirty;
		}
	}
}