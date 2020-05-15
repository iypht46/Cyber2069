#include <glm/glm.hpp>

#include "Graphic/Window.hpp"
#include "Core/Logger.hpp"
#include "Core/GameInfo.h" //TODO: Remove this after message system is done
#include <SDL_surface.h>
#include <SDL_image.h>
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

		void RecenterWindow(void)
		{
			const GLFWvidmode* vidMode = glfwGetVideoMode(glfwMonitor);

			int posX = static_cast<int>((glm::abs(vidMode->width - (windowRes.x)) * 0.15f));
			int posY = static_cast<int>((glm::abs(vidMode->height - (windowRes.y)) * 0.15f));

			glfwSetWindowPos(glfwWindow, posX, posY);

		}

		void SetWindowMode(WindowMode mode)
		{
			const GLFWvidmode* vidMode = glfwGetVideoMode(glfwMonitor);
			switch (mode)
			{
			case Graphic::Window::WindowMode::WINDOWED:

				glfwSetWindowMonitor(glfwWindow, nullptr, 0, 0, windowRes.x, windowRes.y, static_cast<int>(c_refreshRate));
				RecenterWindow();

				break;
			case Graphic::Window::WindowMode::FULLSCREEN:
				
				glfwSetWindowMonitor(glfwWindow, glfwMonitor, 0, 0, windowRes.x, windowRes.y, static_cast<int>(c_refreshRate));
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

		int GetMouseInput(int key)
		{
			return glfwGetMouseButton(glfwWindow, key);
		}

		int GetKeyboardInput(int key)
		{
			return glfwGetKey(glfwWindow, key);
		}

		void SetWindowIcon(const char* path)
		{
			if (!glfwWindow)
			{
				ENGINE_ERROR("Initialize window before setting icon.");
				return;
			}
				

			SDL_Surface* icon = IMG_Load(path);

			if (icon == NULL)
			{
				ENGINE_ERROR("ERROR: Load window icon failed.");
				return;
			}

			GLFWimage* icon_image = new GLFWimage();
			icon_image->width = icon->w;
			icon_image->height = icon->h;
			icon_image->pixels = static_cast<unsigned char*>(icon->pixels);
			glfwSetWindowIcon(glfwWindow, 1, icon_image);

			SDL_FreeSurface(icon);
		}

		void SwapBuffer()
		{
			//Flip Buffers and Draw
			glfwSwapBuffers(glfwWindow);
		}

		bool ShouldClose()
		{
			return static_cast<bool>(glfwWindowShouldClose(glfwWindow));
		}

		void window_close_callback(GLFWwindow* window)
		{
			//TODO: Remove this later and use message system instead
			World::GameInfo::GetInstance().GameShouldClose();

			SetWindowShouldClose(true);
		}

		void Init(const char* title, WindowMode mode, int width, int height)
		{
			windowRes.x = width;
			windowRes.y = height;

			//Init and Configure GLFW:
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			//Create new full screen window
			glfwWindow = glfwCreateWindow(windowRes.x, windowRes.y, title, NULL, NULL);

			if (glfwWindow == NULL)
			{
				//Print out error
				ENGINE_ERROR("Failed Creating GLFW Window");
				//Terminate GLFW
				Terminate();
				return;
			}

			glfwMakeContextCurrent(glfwWindow);

			

			if (glewInit() != GLEW_OK)
			{
				//Print Error
				ENGINE_ERROR("Failed Initializing GLEW");
				return;
			}

			//Set Primary Monitor
			glfwMonitor = glfwGetPrimaryMonitor();
			SetWindowMode(mode);
			glfwSetWindowAspectRatio(glfwWindow, windowRes.x, windowRes.y);
			glfwSetWindowCloseCallback(glfwWindow, window_close_callback);
			glfwSwapInterval(1);
			
			ENGINE_WARN("Window System Initialized");
		}

		void Init(const char* title, WindowMode mode, int size_preset)
		{
			if (size_preset > 3)
			{
				windowRes = windowResArr[3];
			}
			else if (size_preset < 0)
			{
				windowRes = windowResArr[0];
			}
			else
			{
				windowRes = windowResArr[size_preset];
			}
			
			//Init and Configure GLFW:
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			//Create new full screen window
			glfwWindow = glfwCreateWindow(windowRes.x, windowRes.y, title, NULL, NULL);

			if (glfwWindow == NULL)
			{
				//Print out error
				ENGINE_ERROR("Failed Creating GLFW Window");
				//Terminate GLFW
				Terminate();
				return;
			}

			glfwMakeContextCurrent(glfwWindow);



			if (glewInit() != GLEW_OK)
			{
				//Print Error
				ENGINE_ERROR("Failed Initializing GLEW");
				return;
			}

			//Set Primary Monitor
			glfwMonitor = glfwGetPrimaryMonitor();
			SetWindowMode(mode);
			glfwSetWindowAspectRatio(glfwWindow, windowRes.x, windowRes.y);
			glfwSetWindowCloseCallback(glfwWindow, window_close_callback);
			glfwSwapInterval(1);

			ENGINE_WARN("Window System Initialized");
		}

		void Terminate(void)
		{
			glfwDestroyWindow(glfwWindow);
			ENGINE_WARN("Terminate Window and Input System");
		}
	}
}
