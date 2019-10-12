#include "Graphic/GraphicCore.hpp"

#include "Core/Logger.hpp"

namespace Graphic
{
	void Init(void)
	{
		//ENGINE_INFO()
		Window::Init("White Engine", Window::WindowMode::WINDOWED);

		////////////OpenGL////////////
		g_renderer = new GLRenderer(Window::GetWidth(), Window::GetWidth());
		g_renderer->InitGL("Sources/Graphic/Shader/vertext.shd", "Sources/Graphic/Shader/fragment.shd");
		g_renderer->SetOrthoProjection(-1.f, 1.f, -1.f, 1.f);
		g_renderer->SetClearColor(1.0f, 0.0f, 0.0f);

		ENGINE_WARN("Graphic System Initialized");
	}

	void Render(void)
	{
		if (!Window::ShouldClose())
		{
			g_renderer->Render();
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