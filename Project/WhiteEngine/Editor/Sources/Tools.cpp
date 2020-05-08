//Editor
#include "MainEditor.hpp"
//White Engine
#include "Core/GameInfo.h"
#include "Graphic/GraphicCore.hpp"
#include "Input/Input.hpp"
#include "Core/Logger.hpp"
#include "Editor/Editor.hpp"
//Third Party
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
//Standard Library
#include <memory>

namespace Tools
{
	//******Global Variables******//
	const char* glsl_version = "#version 330";
	bool show_demo_window = true;
	bool show_another_window = false;
	
	//BackGround Color
	ImVec4 clear_color = ImVec4(72.0f / 255.0f, 42.0f / 255.0f, 109.0f / 255.0f, 1.00f);
	static std::unique_ptr<MainEditor> mainEditor;

	//Forward Declarations

	void Init(void)
	{
		//Initialize Systems
		//Logger
		Core::Logger::Init();
		//OpenGL and GLFW
		Graphic::Init();
		//Graphic::EnableFrameBuffer(FBO_STATE::SUB);
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

		//Initialize Editor
		mainEditor = std::make_unique<MainEditor>();
	}

	void Update()
	{
		Input::Update();


		mainEditor->Update();
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

}