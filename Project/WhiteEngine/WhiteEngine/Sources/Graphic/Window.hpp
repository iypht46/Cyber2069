#pragma once

//GL Headers
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

//Standard Header
#include <iostream>

namespace Graphic
{
	namespace Window
	{
		enum class WindowMode { WINDOWED = 0, FULLSCREEN };

		////////////System Function////////////
		void Init(const char* title, WindowMode mode);

		void Terminate(void);

		////////////Window Interface////////////
		void SetWindowShouldClose(bool i);

		GLFWwindow* GetWindow(void);

		int GetWidth(void);

		int GetHeight(void);

		void SwapBuffer(void);

		bool ShouldClose(void);

		int GetMouseInput(int);

		int GetKeyboardInput(int);

	}
}

