//System Headers
#include "Input/Input.hpp"
#include "Graphic/Window.hpp"

using namespace Graphic;

namespace Input
{
	glm::vec2 g_mousePosition;

	glm::vec2 g_prevMousePosition;

	void UpdateMousePosition()
	{
		//Get mouse position from glfw
		double xpos, ypos;
		glfwGetCursorPos(Window::GetWindow(), &xpos, &ypos);

		//Update mouse position in Window
		g_prevMousePosition = g_mousePosition;
		g_mousePosition.x = xpos;
		g_mousePosition.y = ypos;

		std::cout << "Mouse POS: " << g_mousePosition.x << " / " << g_mousePosition.y << std::endl;

		std::cout << "Prev Mouse POS: " << g_prevMousePosition.x << " / " << g_prevMousePosition.y << std::endl;
	}

	void UpdateAllInputState()
	{

	}

	glm::vec2 GetMousePosition()
	{
		return g_mousePosition;
	}

	glm::vec2 GetMouseOffset()
	{
		return g_mousePosition - g_prevMousePosition;
	}

	void Init(void)
	{
		//Debug
		std::cout << "Initialize Input System" << std::endl;

	}

	void Update(void)
	{
		//Debug
		std::cout << "Update Input System" << std::endl;

		//Update Key


		//Update Mouse
		UpdateMousePosition();

	}

	void Terminate(void)
	{
		//Debug
		std::cout << "Terminate Input System" << std::endl;
	}




}