#include <Windows.h>
#include "MainEditor.hpp"


namespace Tools
{
	MainEditor::MainEditor()
	{
		this->Init();
	}


	MainEditor::~MainEditor()
	{

	}

	void MainEditor::Init()
	{
		//Set Docking Space


		m_inspector = std::make_unique<Inspector>();
		m_prefabEditor = std::make_unique<PrefabEditor>(&m_prefabBool);
		m_sceneEditor = std::make_unique<SceneEditor>(&m_sceneBool);

	}
	void MainEditor::Update()
	{
		MainMenuBar();
		//TODO: Set up docking space?
		if (m_prefabEditor->isFocused())
			m_currentEditor = m_prefabEditor.get();

		if (m_sceneEditor->isFocused())
			m_currentEditor = m_sceneEditor.get();

		if (m_currentEditor)
		{
			//TODO: Use observer pattern for optimization?
			//if (m_currentEditor->GetSelectedEntity())
			if (m_currentEditor->isFocused() || m_inspector->isFocused())
				m_inspector->SetEntity(m_currentEditor->GetSelectedEntity());
			//else
				//m_inspector->ResetEntity();
			//m_currentEditor->Update();
		}

		if (m_prefabBool)
			m_prefabEditor->Update();

		if (m_sceneBool)
			m_sceneEditor->Update();

		m_inspector->Render();
	}

	void MainEditor::Terminate()
	{
		
	}

	void MainEditor::Save()
	{
		
	}

	void MainEditor::Load(Editor* editor, const char * path)
	{
		std::string returnMsg;
		if (!editor->Load(path, returnMsg.c_str()))
		{
			//Toggle error popup
		}
	}

	void MainEditor::MainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//New file menu
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("Prefab"))
					{
						//Create new prefab editor
						AccessEditor(EDITOR_TYPE::PREFAB_EDITOR);
					}

					if (ImGui::MenuItem("Scene"))
					{
						//Create new scene editor
						AccessEditor(EDITOR_TYPE::SCENE_EDITOR);
					}

					ImGui::EndMenu();
				}

				//Open menu
				if (ImGui::BeginMenu("Open.."))
				{
					if (ImGui::MenuItem("Prefab"))
					{
						if (AccessEditor(EDITOR_TYPE::PREFAB_EDITOR))
						{
							std::string path = openfilename("Engine Files (*.prefab)|*.prefab", NULL);
							//Handle the file

							this->Load(m_prefabEditor.get(),path.c_str());
							
						}

					}

					if (ImGui::MenuItem("Scene"))
					{
						if (AccessEditor(EDITOR_TYPE::SCENE_EDITOR))
						{
							std::string path = openfilename("Scene Files(*.scene)|*.scene", NULL);
							//Handle the file

							this->Load(m_sceneEditor.get(), path.c_str());
						}
					}

					ImGui::EndMenu();
				}
				ImGui::Separator();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y")) {}  // Disabled item
				ImGui::Separator();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				ImGui::MenuItem("Prefab Editor", NULL, &m_prefabBool);
				ImGui::MenuItem("Scene Editor", NULL, &m_sceneBool);
				ImGui::EndMenu();
			}

			if (m_currentEditor)
				m_currentEditor->RenderMenu();

			ImGui::EndMainMenuBar();
		}
	}

	bool MainEditor::AccessEditor(EDITOR_TYPE type)
	{
		//Case: There aren't any editor opened
		switch (type)
		{
		case Tools::EDITOR_TYPE::PREFAB_EDITOR:
			m_prefabBool = true;
			break;
		case Tools::EDITOR_TYPE::SCENE_EDITOR:
			m_sceneBool = true;
			break;
		default:
			return false;
			break;
		}

		return true;
	}

	std::string MainEditor::openfilename(const char *filter = "All Files (*.*)\0*.*\0", HWND owner = NULL)
	{
		OPENFILENAME ofn;
		char fileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = owner;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = fileName;
		ofn.lpstrInitialDir = "D:/GIT/";
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";
		std::string fileNameStr;
		if (GetOpenFileName(&ofn))
			fileNameStr = fileName;
		return fileNameStr;
	}
}


