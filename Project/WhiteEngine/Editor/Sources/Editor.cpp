#include "Editor.hpp"

#include "Graphic/GraphicCore.hpp"
#include "Input/Input.hpp"
#include "Core/Logger.hpp"

namespace Tools
{
	void Editor::Init(void)
	{
		//Initialize Systems
		//OpenGL and GLFW
		Graphic::Init();
		//Input
		Input::Init(false);
		//Logger
		Core::Logger::Init();
	}
}