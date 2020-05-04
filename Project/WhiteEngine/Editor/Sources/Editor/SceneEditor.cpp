#include "SceneEditor.hpp"
#include "Core/EC/GameObject.hpp"
#include "Serialization/Serialization.h"

namespace Tools
{
	SceneEditor::SceneEditor(bool * isOpen = new bool(false)) : Editor("Scene", isOpen) 
	{
		this->Init();
	}

	SceneEditor::SceneEditor() : Editor("Scene")
	{
		this->Init();
	}

	void SceneEditor::Init(void)
	{
		m_gameplayWindow = make_unique<GameplayWindow>();
		m_sceneWindow = make_unique<SceneWindow>(&m_sceneBool);
		m_fileDialog = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CloseOnEsc
			| ImGuiFileBrowserFlags_CreateNewDir);
		m_fileDialog->SetTitle("Scene Save/Load Directory");
		m_fileDialog->SetTypeFilters({ ".Scene" });
	}

	void SceneEditor::OnUpdate(void)
	{
		if (!*m_open)
			return;

		//If user attempt to close window
		if (!m_sceneBool)
		{
			if (m_dirtyFlag)
			{
				//Add to pop up list and attach callback to popup
			}
			else
			{
				m_sceneBool = true;
				//Toggle Modals
				//Add to pop up list and attach callback to popup
			}
		}

		//m_gameplayWindow->Render();
		m_selectedEntity = dynamic_cast<EditorEntity*>(m_sceneObject->GetSelected());
		m_sceneWindow->Render();
	}

	void SceneEditor::Terminate(void)
	{

	}

	bool SceneEditor::Save(Utility::fs::path path, Container::wString& returnMessage)
	{
		bool result = false;
		if (path.has_filename())
		{
			path.replace_extension(".Scene");
			result = m_sceneObject->Save(path.generic_string());
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

	bool SceneEditor::Load(Utility::fs::path path, Container::wString& returnMessage)
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
				//m_sceneObject.reset(); //Clear entity
				//m_sceneObject = make_unique<EditorSceneObject>(); //Make new entity

				result = m_sceneObject->Load(path.generic_string());
				returnMessage = (result) ? (returnMessage + "Success") : (returnMessage + "Failed");
			}
		}

		return result;
	}

	bool SceneEditor::New()
	{
		if (m_sceneObject)
		{
			//Toggle popup
			return false;
		}
		else
		{
			m_sceneObject = make_unique<EditorSceneObject>();
			return true;
		}
	}

	bool SceneEditor::IsFocused()
	{
		return m_sceneWindow->isFocused();
	}

	void SceneEditor::RenderMenu()
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

	EditorObject * SceneEditor::GetEditorObject()
	{
		if (m_sceneObject)
			return m_sceneObject.get();
		else
			return nullptr;
	}

	bool SceneEditor::LoadScene(std::string path)
	{
		Serialization::LoadObject(*m_sceneObject->GetSceneObject(), path);
		
		if (m_sceneObject->GetSceneObject())
			return true;
		else
			return false;
	}

	bool SceneEditor::SaveScene(std::string path)
	{
		if (m_sceneObject->GetSceneObject())
			Serialization::SaveObject(*m_sceneObject->GetSceneObject(), path);
		else
			return false;
	}
	/*GameObject * SceneEditor::GetSelectedGameObject(void)
	{
		return m_selectedGameObject;
	}*/
}

