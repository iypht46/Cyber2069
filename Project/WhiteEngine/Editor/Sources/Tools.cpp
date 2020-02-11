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

		ImGui::StyleColorsDark();

		//Bind ImGui with GLFW
		ImGui_ImplGlfw_InitForOpenGL(Graphic::Window::GetWindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		//Prompt for tools selection

	}

	void Update(float dt)
	{
		MainMenuBar();

		//{
		//	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//	if (show_demo_window)
		//		ImGui::ShowDemoWindow(&show_demo_window);

		//	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		//	{
		//		static float f = 0.0f;
		//		static int counter = 0;

		//		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//		ImGui::Checkbox("Another Window", &show_another_window);

		//		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//			counter++;
		//		ImGui::SameLine();
		//		ImGui::Text("counter = %d", counter);

		//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//		ImGui::End();
		//	}

		//	// 3. Show another simple window.
		//	if (show_another_window)
		//	{
		//		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//		ImGui::Text("Hello from another window!");
		//		if (ImGui::Button("Close Me"))
		//			show_another_window = false;
		//		ImGui::End();
		//	}
		//}
		
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

			Update(World::GameInfo::GetInstance().m_deltaTime);

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
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("GameObject"))
					{
						Editor* editor = mainEditor.get();
						if (editor)
						{
							if (editor->HasSaved())
							{
								mainEditor.reset();
								mainEditor = std::make_unique<PrefabEditor>();
							}
							//TODO: Toggle popup to save current 
							else
							{

							}

						}
						else
						{
							mainEditor.reset();
							mainEditor = std::make_unique<PrefabEditor>();
						}
					}

					if (ImGui::MenuItem("Scene"))
					{
						//TODO: Create new scene editor and set display to true
						//Check if current editor is null or is prefab editor
						//If null then new scene editor
						//If prefab editor then toggle popup
						Editor* editor = mainEditor.get();
						if (editor)
						{
							if (editor->HasSaved())
							{
								mainEditor.reset();
								mainEditor = std::make_unique<SceneEditor>();
							}
							//TODO: Toggle popup to save current 
							else
							{

							}

						}
						else
						{
							mainEditor.reset();
							mainEditor = std::make_unique<SceneEditor>();
						}
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Open.."))
				{
					std::string path = openfilename("Engine Files (*.prefab)|*.prefab|Scene Files(*.scene)|*.scene", NULL);
					//Handle the file
					if (!mainEditor->Load(path.c_str()))
					{
						//Toggle error popup
					}
					
				}

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
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";
		std::string fileNameStr;
		if (GetOpenFileName(&ofn))
			fileNameStr = fileName;
		return fileNameStr;
	}
}