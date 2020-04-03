
#include "Core/Logger.hpp"
#include "Container/String.hpp"
#include "PrefabEditor.hpp"
#include "EditorEntity.hpp"

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
		//ENGINE_INFO("Project Path in prefabEditor: " + Utility::fs::current_path().generic_string());
		m_editorEntitiy = make_unique<EditorEntity>();
		m_previewWindow = make_unique<PreviewWindow>(&m_previewBool);
		m_mainWindow = m_previewWindow.get();
		m_previewBool = false;
		m_selectedEntity = m_editorEntitiy.get();
		//m_inspector = make_unique<Inspector>();
		//m_previewWindow = make_unique<PreviewWindow>();
		//Initialize GameObject
		//m_editorEntitiy->AddComponent(); //Add Transform
		//m_editorEntitiy->AddComponent(); //Add MeshRenderer
	}

	void PrefabEditor::Update(void)
	{
		if (!m_open)
			return;

		if (!m_previewBool)
		{
			if (m_dirtyFlag)
			{

			}
			else
			{
				m_previewBool = true;
				//Toggle Modals
			}
		}

		//TODO: Check first if component exist
		auto mesh = m_editorEntitiy->GetComponent("MeshRendererEC");
		if (mesh)
			m_previewWindow->SetMesh(mesh);

		m_previewWindow->Render();
	}

	void PrefabEditor::Terminate(void)
	{

	}

	bool PrefabEditor::LoadPrefab(const char * filename)
	{
		if (m_editorEntitiy)
			m_editorEntitiy.reset();

		
		
		//If load prefab success, set preview texture to it
		//m_previewWindow->SetTexture();

		//Retrieve gameobject from serialize manager?
		return false;
	}

	bool PrefabEditor::SavePrefab(const char * filename)
	{
		


		return false;
	}

	bool PrefabEditor::Save(const char* path, Container::wString& returnMessage)
	{
		if (SavePrefab(path))
		{
			returnMessage = "Prefab Save Successful.\n";
			this->SetDirty(false);
			return true;
		}

		returnMessage = "Prefab Save Failed.\n";

		return false;
	}

	bool PrefabEditor::Load(const char* path, Container::wString& returnMessage)
	{
		Utility::fs::path fsPath(path);
		m_path = make_unique<Path>(Utility::File::GetRelativePath(fsPath, Editor::s_editorBasePath));

		if (LoadPrefab(m_path->generic_string().c_str()))
		{
			returnMessage = "Load Prefab Success\n";
			this->SetDirty(false);
			return true;
		}
		else
		{
			returnMessage = "Failed Loading Prefab.\n";
			m_path.reset();
			return false;
		}
		/*ENGINE_INFO("Relative Path: " + m_path.generic_string());
		ENGINE_INFO("Current Path: " + Utility::fs::current_path().generic_string());
		ENGINE_INFO("Absolute Path: " + fsPath.generic_string());
		ENGINE_INFO(".relative_path Path: " + fsPath.relative_path().generic_string());*/
		

		/*if (FileBrowser::GetInstance().OpenFileDir(pathStr))
		{
			if (LoadPrefab(pathStr.c_str()))
			{
				return true;
			}
			else
			{
				returnMessage = "Failed Loading Prefab.\n";
			}

			return false;
		}*/
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
			
			}

			ImGui::EndMenu();
		}
	}
	EditorObject * PrefabEditor::GetEditorObject()
	{
		return m_editorEntitiy.get();
	}
}

