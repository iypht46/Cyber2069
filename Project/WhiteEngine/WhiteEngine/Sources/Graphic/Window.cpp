#include "Graphic/Window.hpp"

using namespace glm;

namespace Graphic
{
	namespace Window
	{
		constexpr float c_refreshRate = 60.0f;

		//Windows Var
		GLFWwindow* glfwWindow;
		GLFWmonitor* glfwMonitor;

		const glm::uvec2 windowResArr[] = { {1280, 720}, {1366,768}, {1600, 900}, {1920, 1080} };
		glm::uvec2 windowRes;

		void SetWindowMode(WindowMode mode)
		{
			switch (mode)
			{
			case Graphic::Window::WindowMode::WINDOWED:
				glfwSetWindowMonitor(glfwWindow, nullptr, 0, 0, windowRes.x, windowRes.y, c_refreshRate);
				break;
			case Graphic::Window::WindowMode::FULLSCREEN:
				const GLFWvidmode* vidMode = glfwGetVideoMode(glfwMonitor);
				glfwSetWindowMonitor(glfwWindow, glfwMonitor, 0, 0, windowRes.x, windowRes.y, c_refreshRate);
				break;
			}
		}

		void SetWindowShouldClose(bool i)
		{
			glfwSetWindowShouldClose(glfwWindow, i);
		}

		GLFWwindow* GetWindow()
		{
			return glfwWindow;
		}

		int GetWidth()
		{
			return windowRes.x;
		}

		int GetHeight()
		{
			return windowRes.y;
		}

		void window_close_callback(GLFWwindow* window)
		{
			SetWindowShouldClose(true);
		}

		void Init(const char* title, WindowMode mode)
		{
			windowRes = windowResArr[0];

			//Init and Configure GLFW:
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			

			//Create new full screen window
			glfwWindow = glfwCreateWindow(windowRes.x, windowRes.y, title, NULL, NULL);

			if (glfwWindow == NULL)
			{
				//Print out error
				std::cout << "Failed Creating GLFW Window" << std::endl;
				//Terminate GLFW
				Terminate();
				return;
			}

			glfwMakeContextCurrent(glfwWindow);

			if (glewInit() != GLEW_OK)
			{
				//Print Error
				std::cout << "Failed Initializing GLEW" << std::endl;
				return;
			}

			//Set Primary Monitor
			glfwMonitor = glfwGetPrimaryMonitor();
			SetWindowMode(mode);

			glfwSetWindowCloseCallback(glfwWindow, window_close_callback);

			glfwSetWindowAspectRatio(glfwWindow, windowRes.x, windowRes.y);
		}

		void Terminate(void)
		{
			glfwDestroyWindow(glfwWindow);
		}
	}
}
