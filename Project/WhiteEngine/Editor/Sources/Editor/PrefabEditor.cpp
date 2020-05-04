#include "PrefabEditor.hpp"
#include "EditorObject/EditorEntity.hpp"
#include "EditorWindow/PopupWindow.hpp"
//White Engine
#include "Core/Logger.hpp"
#include "Container/String.hpp"
#include "Serialization/Serialization.h"

#include <iostream>

namespace Tools
{
	PrefabEditor::PrefabEditor(bool* isOpen) : Editor("Prefab", isOpen)
	{
		this->Init();
	}

	PrefabEditor::PrefabEditor() : Editor("Prefab")
	{
		this->Init();
	}

	void PrefabEditor::Init(void)
	{
		//Window Setup
		m_previewWindow = make_unique<PreviewWindow>(&m_previewBool, "Prefab Preview");
		m_mainWindow = m_previewWindow.get();

		//File Dialog Setup
		m_fileDialog = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CloseOnEsc
			| ImGuiFileBrowserFlags_CreateNewDir);
		m_fileDialog->SetTitle("Prefab Save/Load Directory");
		m_fileDialog->SetTypeFilters({ ".Prefab" });

		//Editor object setup
		m_previewBool = false;
		m_selectedEntity = m_entityObject.get();
	}

	void PrefabEditor::OnUpdate(void)
	{
		if (!*m_open)
			return;

		m_selectedEntity = dynamic_cast<EditorEntity*>(m_entityObject->GetSelected());

		if (!m_previewBool)
		{
			if (m_dirtyFlag)
			{
				//Add to pop up list and attach callback to popup
			}
			else
			{
				m_previewBool = true;
				//Toggle Modals
				//Add to pop up list and attach callback to popup
			}
		}

		//TODO: Check first if component exist
		/*auto mesh = m_editorEntitiy->GetComponent("MeshRendererEC");
		if (mesh)
			m_previewWindow->SetMesh(mesh);*/
		m_previewWindow->SetEntity(m_entityObject.get());
		m_previewWindow->Render();
	}

	void PrefabEditor::Terminate(void)
	{

	}

	bool PrefabEditor::Save(Utility::fs::path path, Container::wString& returnMessage)
	{
		bool result = false;
		if (path.has_filename())
		{
			path.replace_extension("." + m_editorName);
			result = m_entityObject->Save(path.generic_string());
			if (result)
			{

				std::cout << path << std::endl;
				returnMessage += "Success";
				this->SetDirty(false);
			}
			else
			{
				returnMessage += "Failed";
				this->SetDirty(true);
			}
		}

		return result;
	}

	bool PrefabEditor::Load(Utility::fs::path path, Container::wString& returnMessage)
	{
		bool result = false;
		if (path.has_filename() && Utility::fs::is_regular_file(path))
		{
			//If object is dirty
			if (this->IsDirty())
			{
				//Send to close queue

			}
			else
			{
				m_entityObject.reset(); //Clear entity
				m_entityObject = make_unique<EditorEntity>(); //Make new entity

				result = m_entityObject->Load(path.generic_string());
				returnMessage = (result) ? (returnMessage + "Success") : (returnMessage + "Failed");
			}
		}

		return result;
	}
	
	bool PrefabEditor::New()
	{
		if (m_entityObject)
		{
			//Toggle Popup Modal
			return false;
		}
		else
		{
			m_entityObject = make_unique<EditorEntity>();
			//m_previewWindow->SetEntity(m_entityObject.get());
			return true;
		}
		
	}
	bool PrefabEditor::IsFocused()
	{
		if (m_previewWindow->isFocused())
		{
			return true;
		}
		
		return false;
	}

	void PrefabEditor::RenderMenu()
	{
		if (ImGui::BeginMenu((this->GetName() + " Editor").c_str()))
		{
			if (ImGui::MenuItem("Save"))
			{
				m_action = EDITOR_ACTION::EDITOR_SAVE;
				m_fileDialog->Open();
			}

			if (ImGui::MenuItem("Load"))
			{
				m_action = EDITOR_ACTION::EDITOR_LOAD;
				m_fileDialog->Open();
			}

			ImGui::EndMenu();
		}
	}
	EditorObject * PrefabEditor::GetEditorObject()
	{
		return m_entityObject.get();
	}
}

