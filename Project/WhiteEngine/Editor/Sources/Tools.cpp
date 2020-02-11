//Standard Library
#include <Windows.h>
#include <memory>
//Tools System
#include "Editor.hpp"
#include "PrefabEditor.hpp"
#include "SceneEditor.hpp"

//Engine Systems
#include "Core/GameInfo.h"
#include "Graphic/GraphicCore.hpp"
#include "Input/Input.hpp"
#include "Core/Logger.hpp"
#include "Editor.hpp"
//ImGUI
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace Tools
{
	using EditorHandle = std::unique_ptr<Editor>;

	
	//Global Variables
	const char* glsl_version = "#version 330";
	bool show_demo_window = true;
	bool show_another_window = false;
	
	//BackGround Color
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	static EditorHandle mainEditor;

	//Forward Declarations
	void MainMenuBar();
	bool AccessEditor(EDITOR_TYPE type);
	std::string openfilename(const char*, HWND owner);

	void Init(void)
	{
		//Initialize Systems
		//Logger
		Core::Logger::Init();
		//OpenGL and GLFW
		Graphic::Init();
		//Input
		Input::Init(false);

		//Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		//Bind ImGui with GLFW
		ImGui_ImplGlfw_InitForOpenGL(Graphic::Window::GetWindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		//Prompt for tools selection

	}

	void Update()
	{
		//MainMenuBar();

		{
			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			//if (show_demo_window)
			//	ImGui::ShowDemoWindow(&show_demo_window);

			//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
			//{
			//	static float f = 0.0f;
			//	static int counter = 0;

			//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			//	ImGui::Checkbox("Another Window", &show_another_window);

			//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			//		counter++;
			//	ImGui::SameLine();
			//	ImGui::Text("counter = %d", counter);

			//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//	ImGui::End();
			//}

			//// 3. Show another simple window.
			//if (show_another_window)
			//{
			//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			//	ImGui::Text("Hello from another window!");
			//	if (ImGui::Button("Close Me"))
			//		show_another_window = false;
			//	ImGui::End();
			//}
		}
		
		//Update main editor
		if (mainEditor)
		{
			mainEditor->Update();
		}
			
	}

	void Loop(void)
	{
		while (!Graphic::Window::ShouldClose())
		{
			//Start frame
			World::GameInfo::GetInstance().StartFrame();

			//Poll Events
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Update();

			if (Input::GetKeyDown(Input::KeyCode::KEY_ESCAPE))
			{
				World::GameInfo::GetInstance().GameShouldClose();
				Graphic::Window::SetWindowShouldClose(true);
			}

			//Rendering
			ImGui::Render();
			Graphic::g_renderer->SetViewPort(0, 0, Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
			Graphic::g_renderer->SetClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			Graphic::Window::SwapBuffer();

			//End Frame
			World::GameInfo::GetInstance().EndFrame();
		}
	}

	void Terminate(void)
	{
		//Terminate ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		//Terminate Input
		Input::Terminate();
		//Terminate Graphic
		Graphic::Terminate();
		//Terminate Game Info
		ENGINE_WARN("Engine Terminated");
	}
	
	void MainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//New file menu
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("GameObject"))
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
					if (ImGui::MenuItem("GameObject"))
					{
						if (AccessEditor(EDITOR_TYPE::PREFAB_EDITOR))
						{
							std::string path = openfilename("Engine Files (*.prefab)|*.prefab", NULL);
							//Handle the file

							if (!mainEditor->Load(path.c_str()))
							{
								//Toggle error popup
							}
						}
						
					}
					if (ImGui::MenuItem("Scene"))
					{
						if (AccessEditor(EDITOR_TYPE::SCENE_EDITOR))
						{
							std::string path = openfilename("Scene Files(*.scene)|*.scene", NULL);
							//Handle the file

							if (!mainEditor->Load(path.c_str()))
							{
								//Toggle error popup
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

			ImGui::EndMainMenuBar();
		}
	}

	std::string openfilename(const char *filter = "All Files (*.*)\0*.*\0", HWND owner = NULL) 
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

	bool AccessEditor(EDITOR_TYPE type)
	{
		//Case: There aren't any editor opened
		if (!mainEditor)
		{
			mainEditor.reset();
			switch (type)
			{
			case Tools::EDITOR_TYPE::PREFAB_EDITOR:
				mainEditor = std::make_unique<PrefabEditor>();
				break;
			case Tools::EDITOR_TYPE::SCENE_EDITOR:
				mainEditor = std::make_unique<SceneEditor>();
				break;
			default:
				break;
			}
			return true;
		}

		//An editor is opened
		//Check if editor has been saved
		if (mainEditor->HasSaved())
		{
			mainEditor.reset();
			switch (type)
			{
			case Tools::EDITOR_TYPE::PREFAB_EDITOR:
				mainEditor = std::make_unique<PrefabEditor>();
				break;
			case Tools::EDITOR_TYPE::SCENE_EDITOR:
				mainEditor = std::make_unique<SceneEditor>();
				break;
			default:
				break;
			}
			return true;
		}
		//TODO: Toggle popup to save current

		return false;
	}


}