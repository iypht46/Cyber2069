#pragma once

//System Headers
#include "Graphic/Window.hpp"
#include "Graphic/GLRenderer.h"

//Standard Header
#include <iostream>

namespace Graphic
{
	static GLRenderer* g_renderer;

	void Init(std::string windowName, int width = 1280, int height = 720, Window::WindowMode mode = Window::WindowMode::WINDOWED);
	void Init(std::string windowName, int size_preset = 0, Window::WindowMode mode = Window::WindowMode::WINDOWED);

	void Render(void);

	void Terminate(void);
}