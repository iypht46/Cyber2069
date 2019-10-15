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
		g_renderer->SetOrthoProjection(-Window::GetWidth() / 2, Window::GetWidth() / 2, -Window::GetHeight() / 2, Window::GetHeight() / 2);
		g_renderer->SetClearColor(0.2f, 0.3f, 0.3f);

		std::cout << "Initialize OpenGL" << std::endl;

		g_renderer->test = new MeshRenderer("Sources/Rabbit.png", 1, 1);
		
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