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
	}
	void MainEditor::Update()
	{
		MainMenuBar();
		//TODO: Set up docking space?

		if (m_currentEditor)
		{
			//TODO: Use observer pattern for optimization?
			m_inspector->SetEntity(m_currentEditor->GetSelectedEntity());
			m_currentEditor->Update();
		}

		m_inspector->Render();
	}

	void MainEditor::Terminate()
	{
		
	}

	void MainEditor::Save()
	{
		
	}

	void MainEditor::Load(const char * path)
	{
		std::string returnMsg;
		if (!m_currentEditor->Load(path, returnMsg.c_str()))
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

							this->Load(path.c_str());
							
						}

					}
					if (ImGui::MenuItem("Scene"))
					{
						if (AccessEditor(EDITOR_TYPE::SCENE_EDITOR))
						{
							std::string path = openfilename("Scene Files(*.scene)|*.scene", NULL);
							//Handle the file

							this->Load(path.c_str());
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

			ImGui::EndMainMenuBar();
		}
	}

	bool MainEditor::AccessEditor(EDITOR_TYPE type)
	{
		//Case: There aren't any editor opened
		
		if (!m_currentEditor)
		{
			m_currentEditor.reset();
			switch (type)
			{
			case Tools::EDITOR_TYPE::PREFAB_EDITOR:
				m_currentEditor = std::make_unique<PrefabEditor>();
				break;
			case Tools::EDITOR_TYPE::SCENE_EDITOR:
				m_currentEditor = std::make_unique<SceneEditor>();
				break;
			default:
				break;
			}
			return true;
		}

		//An editor is opened
		//Check if editor has been saved
		if (m_currentEditor->HasSaved())
		{
			m_currentEditor.reset();
			switch (type)
			{
			case Tools::EDITOR_TYPE::PREFAB_EDITOR:
				m_currentEditor = std::make_unique<PrefabEditor>();
				break;
			case Tools::EDITOR_TYPE::SCENE_EDITOR:
				m_currentEditor = std::make_unique<SceneEditor>();
				break;
			default:
				break;
			}
			return true;
		}
		//TODO: Toggle popup to save current

		return false;
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


