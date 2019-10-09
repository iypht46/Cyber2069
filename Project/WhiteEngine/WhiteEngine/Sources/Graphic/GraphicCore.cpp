#include "Graphic/GraphicCore.hpp"

namespace Graphic
{
	void Init(void)
	{
		std::cout << "Initialize Graphic System" << std::endl;
		Window::Init("White Engine", Window::WindowMode::WINDOWED);

		////////////OpenGL////////////
		g_renderer = new GLRenderer(Window::GetWidth(), Window::GetWidth());
		g_renderer->InitGL("Sources/Graphic/Shader/vertext.shd", "Sources/Graphic/Shader/fragment.shd");
		g_renderer->SetOrthoProjection(-1.f, 1.f, -1.f, 1.f);
		g_renderer->SetClearColor(1.0f, 0.0f, 0.0f);

		std::cout << "Initialize OpenGL" << std::endl;
		
	}

	void Render(void)
	{
		g_renderer->Render();
		glfwSwapBuffers(Window::GetWindow());
	}

	void Terminate(void)
	{
		//Terminate Window
		Window::Terminate();
		//Terminate Glfw
		glfwTerminate();
	}
}