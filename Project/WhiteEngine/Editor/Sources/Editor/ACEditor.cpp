#include "ACEditor.hpp"

namespace Tools
{
	ACEditor::ACEditor(bool* isOpen) : Editor("AnimationController", isOpen)
	{
		this->Init();
	}

	ACEditor::ACEditor() : Editor("AnimationController")
	{
		this->Init();
	}

	void ACEditor::Init(void)
	{
		//Initialize Entity and Component
		m_entityACTester = std::make_unique<EditorEntity>();
		m_entityACTester->AddComponent("MeshRendererEC");
		m_entityACTester->AddComponent("AnimatorEC");
		m_animatorEC = dynamic_cast<AnimatorEC*>(m_entityACTester->GetComponent("AnimatorEC"));
		m_meshEC = dynamic_cast<MeshRendererEC*>(m_entityACTester->GetComponent("MeshRendererEC"));

		//Initialize Window
		m_previewWindow = std::make_unique<PreviewWindow>(&m_previewBool, "Animation Preview", false, 400, 400);
		m_previewWindow->SetEntity(m_entityACTester.get());
		m_animationWindow = std::make_unique<AnimationWindow>();
		m_animationWindow->SetEditorObject(m_acObject.get());
		m_animationWindow->SetEditorEntity(m_entityACTester.get());
		m_mainWindow = m_previewWindow.get(); //Main window that will show saved/unsaved in title

		//Initialize File dialog window
		m_fileDialog = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename 
			| ImGuiFileBrowserFlags_CloseOnEsc
			| ImGuiFileBrowserFlags_CreateNewDir);
		m_fileDialog->SetTitle("Scene Save/Load Directory");
		m_fileDialog->SetTypeFilters({ ".animcon" });
	}

	void ACEditor::Terminate(void)
	{
	}

	void ACEditor::OnUpdate(void)
	{
		if (!m_open)
			return;

		//If user attempt to close window
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
		else
		{
			m_previewWindow->Render();
		}

		m_animationWindow->Render();

		if (m_acObject)
		{
			m_animationWindow->SetEditorObject(m_acObject.get());
			

			if (m_acObject->currentState != -1)
			{
				m_animatorEC->SetCurrentState(m_acObject->currentState);
				m_acObject->currentState = -1;
			}

			if (m_acObject->shouldUpdateMesh && m_meshEC)
			{
				auto mesh = m_entityACTester->GetGameObject()->GetComponent<MeshRenderer>();

				if (mesh)
				{
					glm::vec2 frame_size = m_acObject->GetAnimController()->getSheetSize();
					mesh->CreateMesh(frame_size.x, frame_size.y);
				}

				m_acObject->shouldUpdateMesh = false;
			}
		}

		
	}

	bool ACEditor::IsFocused()
	{
		return (m_previewWindow->isFocused() || m_animationWindow->isFocused());
	}

	void ACEditor::RenderMenu()
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

			if (ImGui::MenuItem("New"))
			{
				this->New();
			}

			ImGui::EndMenu();
		}
	}

	bool ACEditor::Save(Utility::fs::path path, Container::wString& returnMessage)
	{
		bool result = false;
		if (path.has_filename())
		{
			path.replace_extension(".animcon");
			result = m_acObject->Save(path.generic_string());
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

	bool ACEditor::Load(Utility::fs::path path, Container::wString& returnMessage)
	{
		if (!m_meshEC->IsTextureLoaded())
		{
			returnMessage = "Load mesh first before loading animation controller!!";
			return false;
		}

		bool result = false;
		if (path.has_filename() && Utility::fs::is_regular_file(path))
		{
			//If object is dirty
			if (this->IsDirty())
			{
				//Send to close queue
				returnMessage = "Animation controller is not saved";
			}
			else
			{
				//m_sceneObject.reset(); //Clear entity
				//m_sceneObject = make_unique<EditorSceneObject>(); //Make new entity
				if (!m_acObject)
					this->New();

				result = m_acObject->Load(path.generic_string());
				m_animatorEC->SetController(m_acObject->GetAnimController());
				returnMessage = (result) ? (returnMessage + "Success") : (returnMessage + "Failed");
			}
		}

		return result;
	}

	bool ACEditor::New()
	{
		m_acObject.reset();
		m_acObject = make_unique<EditorAC>();
		m_animationWindow->ResetEditorObject();

		if (m_animatorEC)
			m_animatorEC->SetController(m_acObject->GetAnimController());

		return true;

		//Use this to prevent overriding unsaved changes
		//if (m_acObject)
		//{
		//	//Toggle popup
		//	return false;
		//}
		//else
		//{
		//	m_acObject = make_unique<EditorAC>();
		//	
		//	if (m_animatorEC)
		//		m_animatorEC->SetController(m_acObject->GetAnimController());
		//	return true;
		//}
	}
}