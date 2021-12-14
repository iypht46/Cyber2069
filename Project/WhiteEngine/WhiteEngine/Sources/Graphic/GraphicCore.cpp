#include "Graphic/GraphicCore.hpp"
#include "Camera.hpp"
#include "Core/Logger.hpp"


namespace Graphic
{

	void InitGL()
	{
		////////////OpenGL////////////
		g_renderer = GLRenderer::GetInstance();
		g_renderer->InitGL("Sources/Shader/vertext.shd", "Sources/Shader/fragment.shd");
		g_renderer->SetOrthoProjection(-Window::GetWidth() / 2, Window::GetWidth() / 2, -Window::GetHeight() / 2, Window::GetHeight() / 2);
		g_renderer->SetClearColor(72.0f / 255.0f, 42.0f / 255.0f, 109.0f / 255.0f);

		getCamera()->ResetCam();
	}

	void Init(std::string windowName, int width, int height, Window::WindowMode mode)
	{
		Window::Init(windowName.c_str(), mode, width, height);

		InitGL();

		ENGINE_WARN("Graphic System Initialized");
	}

	void Init(std::string windowName, int size_preset, Window::WindowMode mode)
	{
		Window::Init(windowName.c_str(), mode, size_preset);

		InitGL();

		ENGINE_WARN("Graphic System Initialized");
	}

	void Render(void)
	{
		if (!Window::ShouldClose())
		{
			g_renderer->Render(getCamera());
			Window::SwapBuffer();
			glfwPollEvents();
		}
	}

	void Terminate(void)
	{
		//Terminate Window
		Window::Terminate();
		//Terminate Glfw
		glfwTerminate();

		ENGINE_WARN("Graphic System Terminated");
	}
}
