#include "Editor.hpp"

namespace Tools
{
	Utility::fs::path Editor::s_editorBasePath = Utility::fs::current_path();

	Editor::Editor(Container::wString editor_name, bool* isOpen) : m_editorName(editor_name), m_open(isOpen)
	{
		//m_fileDialog->SetPwd(s_editorBasePath);
	}

	Container::wString Editor::GetName()
	{
		return m_editorName;
	}

	bool Editor::HasSaved() { return m_dirtyFlag; }

	void Editor::Update(void)
	{
		OnUpdate();

		if (m_fileDialog)
		{
			m_fileDialog->Display();

			if (!m_fileDialog->IsOpened())
			{
				m_action = EDITOR_ACTION::EDITOR_NONE;
			}

			if (m_fileDialog->HasSelected() && m_action != EDITOR_ACTION::EDITOR_NONE)
			{
				std::string returnMsg;
				if (m_action == EDITOR_ACTION::EDITOR_SAVE)
				{
					returnMsg += "Save " + m_editorName + " ";
					Save(m_fileDialog->GetSelected(), returnMsg);
					//if (Save(m_fileDialog->GetSelected(), returnMsg))
					//{
					//	//Save Successful
					//}
					//else
					//{
					//	//Save Failed
					//}

					PopupData newData("Save :", returnMsg);
					m_popup.Push(newData);
				}
				else
				{
					returnMsg += "Load " + m_editorName + " ";
					Load(m_fileDialog->GetSelected(), returnMsg);
					//if (Load(m_fileDialog->GetSelected(), returnMsg))
					//{
					//	//Load Successful
					//}
					//else
					//{
					//	//Load Failed
					//}

					PopupData newData("Load :", returnMsg);
					m_popup.Push(newData);
				}

				m_action = EDITOR_ACTION::EDITOR_NONE;
				m_fileDialog->ClearSelected();
				ENGINE_INFO(returnMsg);
			}

			
		}

		m_popup.Update();
	}

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

	void Editor::SetAction(EDITOR_ACTION action)
	{
		m_action = action;
	}

	void Editor::SetFileDialog(bool i)
	{
		if (!m_fileDialog)
			return;

		if (i)
			m_fileDialog->Open();
		else
			m_fileDialog->Close();
	}
}