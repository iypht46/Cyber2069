#include <Windows.h>
#include <filesystem>
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
		m_editorMap[EDITOR_TYPE::PREFAB_EDITOR] = std::make_unique<PrefabEditor>();
		m_editorMap[EDITOR_TYPE::SCENE_EDITOR] = std::make_unique<SceneEditor>();
		m_acEditor = std::make_unique<ACEditor>(&m_acBool);
		m_particleEditor = std::make_unique<ParticleEditor>(&m_particleBool);
		m_inspector = std::make_unique<Inspector>();
		m_hierarchy = std::make_unique<ObjectHierarchy>();
	}

	void MainEditor::Update()
	{
		MainMenuBar();
		//TODO: Set up docking space?

		for (auto it = m_editorMap.begin(); it != m_editorMap.end(); it++)
		{
			auto editor = it->second.get();

			if (editor->IsFocused() && editor->IsOpen())
				m_currentEditor = editor;

			/*if (editor->IsOpen())
			{
				editor->Update();
				ENGINE_INFO("Update Editor: " + editor->GetName());
			}*/
				
		}

		/*if (m_prefabEditor->IsFocused() && m_prefabEditor->IsOpen()) m_currentEditor = m_prefabEditor.get();
		if (m_sceneEditor->IsFocused() && m_sceneEditor->IsOpen()) m_currentEditor = m_sceneEditor.get();

		if (m_prefabEditor->IsOpen())
			m_prefabEditor->Update();

		if (m_sceneEditor->IsOpen())
			m_sceneEditor->Update();*/

		//Update after setting object to inspector and hierarchy
		for (auto it = m_editorMap.begin(); it != m_editorMap.end(); it++)
		{
			auto editor = it->second.get();

			if (editor->IsOpen())
			{
				editor->Update();
				/*ENGINE_INFO("Update Editor: " + editor->GetName());*/
			}

		}

		if (m_currentEditor)
		{
			//TODO: Use observer pattern for optimization?
			//if (m_currentEditor->GetSelectedEntity())
			if (m_currentEditor->IsFocused() || m_inspector->isFocused() || m_hierarchy->isFocused())
			{
				m_inspector->SetEditorObject(m_currentEditor->GetSelectedEntity());
				m_hierarchy->SetEditorObject(m_currentEditor->GetEditorObject());

				//m_hierarchy->SetEditorObject();
				//m_inspector->SetEntity(m_currentEditor->GetSelectedEntity());
			}
				
			//else
				//m_inspector->ResetEntity();
			//m_currentEditor->Update();
		}

		

		if (m_inspector->GetBool())
			m_inspector->Render();

		if (m_acBool)
			m_acEditor->Update();

		if (m_particleBool)
			m_particleEditor->Update();

		if (m_hierarchy->GetBool())
			m_hierarchy->Render();

		
	}

	void MainEditor::Terminate()
	{
		
	}

	void MainEditor::Save(Editor* editor, Container::wString fileName)
	{
		
	}

	void MainEditor::Load(Editor* editor, const char * path)
	{
		try
		{
			std::string returnMsg;
			if (!editor->Load(path, returnMsg))
			{
				//Toggle error popup
				/*static bool loadFile;
				ImGuiWindowFlags loadFileModalFlag = ImGuiWindowFlags_Modal;
				ImGui::BeginPopupModal("#Load File", &loadFile);*/
				throw returnMsg;
			}
		}
		catch (std::string msg)
		{
			ENGINE_ERROR("ERROR: " + msg);
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
					for (auto it = m_editorMap.begin(); it != m_editorMap.end(); it++)
					{
						Editor* editor = it->second.get();

						if (!editor)
							continue;

						if (ImGui::MenuItem(editor->GetName().c_str()))
						{
							if (AccessEditor(editor))
							{
								editor->New();
								ENGINE_INFO("Access Editor: " + editor->GetName());
							}
							else
							{
								//Toggle modal to save first

							}
						}
					}

					ImGui::EndMenu();
				}

				//Open menu
				if (ImGui::BeginMenu("Open.."))
				{
					for (auto it = m_editorMap.begin(); it != m_editorMap.end(); it++)
					{
						Editor* editor = it->second.get();

						if (!editor)
							continue;

						if (ImGui::MenuItem(editor->GetName().c_str()))
						{
							if (AccessEditor(editor))
							{
								Container::wString filter = "Engine Files (*." + editor->GetName() + ")|*." + editor->GetName();

								//std::string path = openfilename(filter.c_str());
								//Handle the file

								//this->Load(editor, path.c_str());
								editor->SetAction(EDITOR_ACTION::EDITOR_LOAD);
								editor->SetFileDialog(true);
							}
							else
							{
								//Toggle modal to save first

							}
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
				for (auto window : UIWindow::GetWindowList())
				{
					bool shouldShow = window->IsClosable();
					if (shouldShow)
					{
						if (ImGui::MenuItem(window->GetName().c_str(), NULL, window->GetBool(), shouldShow))
						{
							window->ToggleBool();
						}
					}
				}

				ImGui::MenuItem(m_acEditor->GetName().c_str(), NULL, &m_acBool, true);

				ImGui::MenuItem(m_particleEditor->GetName().c_str(), NULL, &m_particleBool, true);
				/*ImGui::MenuItem("Prefab Editor", NULL, &m_prefabBool);
				ImGui::MenuItem("Scene Editor", NULL, &m_sceneBool);*/
				ImGui::EndMenu();
			}

			if (m_currentEditor)
				m_currentEditor->RenderMenu();

			if (m_acBool || m_acEditor->IsFocused())
				m_acEditor->RenderMenu();

			if (m_particleBool)
				m_particleEditor->RenderMenu();

			ImGui::EndMainMenuBar();
		}
	}

	bool MainEditor::AccessEditor(Editor* editor)
	{
		//TODO: Check if editor is dirty
		if (!editor->IsOpen())
		{
			editor->SetOpen(true);
			return true;
		}
		else
		{
			if (!editor->IsDirty())
			{
				

				return true;
			}
		}

		//Toggle Popup

		ENGINE_WARN("Editor have not been saved");
		return false;
	}

	bool MainEditor::AccessEditor(EDITOR_TYPE type)
	{
		auto editor = m_editorMap[type].get();

		if (editor)
		{
			if (!editor->IsOpen())
			{
				editor->SetOpen(true);
				return true;
			}
			else
			{
				if (!editor->IsDirty())
				{


					return true;
				}
			}
		}
		else
		{
			ENGINE_ERROR("ERROR: Editor doesn't exist!");
			return false;
		}
	}

	//Open window file dialog, Example Filter format: "Scene Files(*.scene)|*.scene"
	std::string MainEditor::openfilename(const char *filter, HWND owner)
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


