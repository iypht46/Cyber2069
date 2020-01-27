#include "Graphic/GraphicCore.hpp"

#include "Core/Logger.hpp"
#include "Graphic/Camera.hpp"

namespace Graphic
{
	void Init(void)
	{
		//ENGINE_INFO()
		Window::Init("White Engine", Window::WindowMode::WINDOWED);

		////////////OpenGL////////////
		g_renderer = GLRenderer::GetInstance();

		g_renderer->InitGL("Sources/Shader/vertext.shd", "Sources/Shader/fragment.shd");
		g_renderer->SetOrthoProjection(-Window::GetWidth() / 2, Window::GetWidth() / 2, -Window::GetHeight() / 2, Window::GetHeight() / 2);
		g_renderer->SetClearColor(72.0f/255.0f, 42.0f / 255.0f, 109.0f / 255.0f);

		Graphic::getCamera()->ResetCam();

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
