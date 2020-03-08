#include "PrefabEditor.hpp"
#include "EditorEntity.hpp"
#include "FileBrowser.hpp"

#include <iostream>

namespace Tools
{
	PrefabEditor::PrefabEditor(bool* isOpen) : Editor(EDITOR_TYPE::PREFAB_EDITOR, isOpen)
	{
		m_editorEntitiy = make_unique<EditorEntity>();
		m_previewWindow = make_unique<PreviewWindow>(&m_previewBool);
		m_selectedEntity = m_editorEntitiy.get();
	}

	void PrefabEditor::Init(void)
	{
		
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
			if (m_hasSaved)
			{

			}
			else
			{
				m_previewBool = true;
				//Toggle Modals
			}
		}

		m_previewWindow->SetMesh(m_editorEntitiy->GetComponent("MeshRendererEC"));
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
		m_hasSaved = true;

		return false;
	}

	bool PrefabEditor::Save(const char* path, const char* returnMessage)
	{
		if (SavePrefab(path))
		{
			returnMessage = "Prefab Save Successful.\n";
			return true;
		}

		returnMessage = "Prefab Save Failed.\n";

		return false;
	}

	bool PrefabEditor::Load(const char* path, const char* returnMessage)
	{
		std::string pathStr(path);
		if (FileBrowser::GetInstance().OpenFileDir(pathStr))
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
		}

		//Pop up: File not found
		returnMessage = "File not found!\n";
		return false;
	}
	bool PrefabEditor::isFocused()
	{
		if (m_previewWindow->isFocused())
		{
			return true;
		}
		
		return false;
	}
}

