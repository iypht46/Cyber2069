#include "SceneEditor.hpp"
#include "Core/EC/GameObject.hpp"

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
	}

	void SceneEditor::Update(void)
	{
		m_gameplayWindow->Render();
	}

	void SceneEditor::Terminate(void)
	{
	}
	bool SceneEditor::Save(const char * path, Container::wString& returnMessage)
	{
		return false;
	}
	bool SceneEditor::Load(const char * path, Container::wString& returnMessage)
	{
		return false;
	}

	bool SceneEditor::IsFocused()
	{
		return m_gameplayWindow->isFocused();
	}

	void SceneEditor::RenderMenu()
	{
		if (ImGui::BeginMenu((this->GetName() + " Editor").c_str()))
		{
			if (ImGui::MenuItem("Save"))
			{

			}

			ImGui::EndMenu();
		}
	}

	EditorObject * SceneEditor::GetEditorObject()
	{
		return m_sceneObject.get();
	}

	bool SceneEditor::LoadScene(const char*)
	{
		return false;
	}
	bool SceneEditor::SaveScene(const char*)
	{
		return false;
	}
	/*GameObject * SceneEditor::GetSelectedGameObject(void)
	{
		return m_selectedGameObject;
	}*/
}

