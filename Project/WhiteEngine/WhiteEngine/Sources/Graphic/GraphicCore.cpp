#include "Graphic/GraphicCore.hpp"

namespace Graphic
{
	void Init(void)
	{
		std::cout << "Initialize Graphic System" << std::endl;
		Window::Init("White Engine", Window::WindowMode::WINDOWED);

		////////////OpenGL////////////
		
	}

	void Render(void)
	{

	}

	void Terminate(void)
	{
		//Terminate Window
		Window::Terminate();
		//Terminate Glfw
		glfwTerminate();
	}
}