#include "Graphic/GraphicCore.hpp"

#include "Core/Logger.hpp"

namespace Graphic
{
	void Init(void)
	{
		//ENGINE_INFO()
		Window::Init("White Engine", Window::WindowMode::WINDOWED);

		////////////OpenGL////////////
		g_renderer = new GLRenderer(Window::GetWidth(), Window::GetHeight());

		g_renderer->InitGL("Sources/Graphic/Shader/vertext.shd", "Sources/Graphic/Shader/fragment.shd");
		g_renderer->SetOrthoProjection(-Window::GetWidth() / 2, Window::GetWidth() / 2, -Window::GetHeight() / 2, Window::GetHeight() / 2);
		g_renderer->SetClearColor(0.2f, 0.3f, 0.3f);

		ENGINE_WARN("Graphic System Initialized");

		//g_renderer->test = new MeshRenderer("Sources/Mockup_PlayerBody_Vversion02.png", 7, 5);

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
