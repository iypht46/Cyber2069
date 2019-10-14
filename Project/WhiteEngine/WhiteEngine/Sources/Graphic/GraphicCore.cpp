#include "Graphic/GraphicCore.hpp"

namespace Graphic
{
	void Init(void)
	{
		std::cout << "Initialize Graphic System" << std::endl;
		Window::Init("White Engine", Window::WindowMode::WINDOWED);

		////////////OpenGL////////////
		g_renderer = new GLRenderer(Window::GetWidth(), Window::GetHeight());

		g_renderer->InitGL("Sources/Graphic/Shader/vertext.shd", "Sources/Graphic/Shader/fragment.shd");
		g_renderer->SetOrthoProjection(-3.f, 3.f, -3.f, 3.f);
		g_renderer->SetClearColor(0.2f, 0.3f, 0.3f);

		std::cout << "Initialize OpenGL" << std::endl;
		
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
	}
}