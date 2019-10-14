//Standard Header
#include <unordered_map>

//System Headers
#include "Input/Input.hpp"
#include "Graphic/Window.hpp"

using namespace Graphic;
using namespace glm;

namespace Input
{
	vec2 g_mousePosition;

	vec2 g_prevMousePosition;

	//Keyboard array for update loop
	KeyCode keyCodeEnumArr[] =
	{
		KeyCode::KEY_A,
		KeyCode::KEY_B,
		KeyCode::KEY_C,
		KeyCode::KEY_D,
		KeyCode::KEY_E,
		KeyCode::KEY_F,
		KeyCode::KEY_G,
		KeyCode::KEY_H,
		KeyCode::KEY_I,
		KeyCode::KEY_J,
		KeyCode::KEY_K,
		KeyCode::KEY_L,
		KeyCode::KEY_M,
		KeyCode::KEY_N,
		KeyCode::KEY_O,
		KeyCode::KEY_P,
		KeyCode::KEY_Q,
		KeyCode::KEY_R,
		KeyCode::KEY_S,
		KeyCode::KEY_T,
		KeyCode::KEY_U,
		KeyCode::KEY_V,
		KeyCode::KEY_W,
		KeyCode::KEY_X,
		KeyCode::KEY_Y,
		KeyCode::KEY_Z,
		KeyCode::KEY_0,
		KeyCode::KEY_1,
		KeyCode::KEY_2,
		KeyCode::KEY_3,
		KeyCode::KEY_4,
		KeyCode::KEY_5,
		KeyCode::KEY_6,
		KeyCode::KEY_7,
		KeyCode::KEY_8,
		KeyCode::KEY_9,
		KeyCode::KEY_SPACE,
		KeyCode::KEY_ENTER,
		KeyCode::KEY_RIGHT,
		KeyCode::KEY_LEFT,
		KeyCode::KEY_DOWN,
		KeyCode::KEY_UP,
		KeyCode::KEY_LEFT_SHIFT,
		KeyCode::KEY_LEFT_CONTROL,
		KeyCode::KEY_LEFT_ALT,
		KeyCode::KEY_ESCAPE,
		KeyCode::KEY_COUNT
	};

	//String for printing
	const char* keyboardCodeName[]
	{
		"KEY_A",
		"KEY_B",
		"KEY_C",
		"KEY_D",
		"KEY_E",
		"KEY_F",
		"KEY_G",
		"KEY_H",
		"KEY_I",
		"KEY_J",
		"KEY_K",
		"KEY_L",
		"KEY_M",
		"KEY_N",
		"KEY_O",
		"KEY_P",
		"KEY_Q",
		"KEY_R",
		"KEY_S",
		"KEY_T",
		"KEY_U",
		"KEY_V",
		"KEY_W",
		"KEY_X",
		"KEY_Y",
		"KEY_Z",
		"KEY_0",
		"KEY_1",
		"KEY_2",
		"KEY_3",
		"KEY_4",
		"KEY_5",
		"KEY_6",
		"KEY_7",
		"KEY_8",
		"KEY_9",
		"KEY_SPACE",
		"KEY_ENTER",
		"KEY_RIGHT",
		"KEY_LEFT",
		"KEY_DOWN",
		"KEY_UP",
		"KEY_LEFT_S",
		"KEY_LEFT_C",
		"KEY_LEFT_A",
		"KEY_ESCAPE",
		"KEY_COUNT",
	};

	//Mouse array for update loop
	MouseKeyCode mouseKeyCodeEnumArr[] =
	{
		MouseKeyCode::MOUSE_LEFT,
		MouseKeyCode::MOUSE_MIDDLE,
		MouseKeyCode::MOUSE_RIGHT
	};

	//String for printing
	const char* mouseKeyCodeName[]
	{
		"MOUSE_BUTTON_LEFT",
		"MOUSE_BUTTON_MIDDLE",
		"MOUSE_BUTTON_RIGHT"
	};

	//Type Def
	using KeyStateMap = std::unordered_map<KeyCode, KeyState>;
	using MouseStateMap = std::unordered_map<MouseKeyCode, KeyState>;
	
	//Declare
	KeyStateMap g_keyStateMap;
	MouseStateMap g_mouseStateMap;
	Animation test;

	////////////Update Function////////////

	void UpdateMousePosition()
	{
		//Get mouse position from glfw
		double xpos, ypos;
		glfwGetCursorPos(Window::GetWindow(), &xpos, &ypos);

		//Update mouse position in Window
		g_prevMousePosition = g_mousePosition;
		g_mousePosition.x = xpos;
		g_mousePosition.y = ypos;

		/*std::cout << "Mouse POS: " << g_mousePosition.x << " / " << g_mousePosition.y << std::endl;

		std::cout << "Prev Mouse POS: " << g_prevMousePosition.x << " / " << g_prevMousePosition.y << std::endl;*/
	}

	void UpdateKeyState(KeyCode key)
	{
		KeyState previousState = g_keyStateMap[key];

		if (Window::GetKeyboardInput(static_cast<int>(key)) == GLFW_PRESS)
		{
			if (previousState == KeyState::KEY_NONE || previousState == KeyState::KEY_UP)
			{
				g_keyStateMap[key] = KeyState::KEY_DOWN;
			}
			else
			{
				g_keyStateMap[key] = KeyState::KEY_HOLD;
			}
		}
		else
		{
			if (previousState == KeyState::KEY_DOWN || previousState == KeyState::KEY_HOLD)
			{
				g_keyStateMap[key] = KeyState::KEY_UP;
			}
			else
			{
				g_keyStateMap[key] = KeyState::KEY_NONE;
			}
		}
	}

	void UpdateKeyState(MouseKeyCode key)
	{
		KeyState previousState = g_mouseStateMap[key];

		if (Window::GetMouseInput(static_cast<int>(key)) == GLFW_PRESS)
		{
			if (previousState == KeyState::KEY_NONE || previousState == KeyState::KEY_UP)
			{
				g_mouseStateMap[key] = KeyState::KEY_DOWN;
			}
			else
			{
				g_mouseStateMap[key] = KeyState::KEY_HOLD;
			}
		}
		else
		{
			if (previousState == KeyState::KEY_DOWN || previousState == KeyState::KEY_HOLD)
			{
				g_mouseStateMap[key] = KeyState::KEY_UP;
			}
			else
			{
				g_mouseStateMap[key] = KeyState::KEY_NONE;
			}
		}
	}

	void UpdateAllInputState()
	{
		//Update Keyboard State
		for (auto keyboardState : keyCodeEnumArr)
		{
			UpdateKeyState(keyboardState);
		}

		//Update Mouse State
		for (auto mouseState : mouseKeyCodeEnumArr)
		{
			UpdateKeyState(mouseState);
		}

	}

	////////////Mouse Interface////////////

	glm::vec2 GetMousePosition()
	{
		return g_mousePosition;
	}

	glm::vec2 GetMouseOffset()
	{
		return g_mousePosition - g_prevMousePosition;
	}

	KeyState GetKeyState(KeyCode keyCode)
	{
		return g_keyStateMap[keyCode];
	}

	KeyState GetMouseState(MouseKeyCode keyCode)
	{
		return g_mouseStateMap[keyCode];
	}

	bool GetMouseUp(MouseKeyCode keyCode)
	{
		return GetMouseState(keyCode) == KeyState::KEY_UP;
	}

	bool GetMouseUp(unsigned keyCode)
	{
		return GetMouseUp(static_cast<MouseKeyCode>(keyCode));
	}

	bool GetMouseDown(MouseKeyCode keyCode)
	{
		return GetMouseState(keyCode) == KeyState::KEY_DOWN;
	}

	bool GetMouseDown(unsigned keyCode)
	{
		return GetMouseDown(static_cast<MouseKeyCode>(keyCode));
	}

	bool GetMouseHold(MouseKeyCode keyCode)
	{
		return GetMouseState(keyCode) == KeyState::KEY_HOLD;
	}

	bool GetMouseHold(unsigned keyCode)
	{
		return GetMouseHold(static_cast<MouseKeyCode>(keyCode));
	}

	////////////Key Interface////////////

	bool GetKeyUp(KeyCode keyCode)
	{
		return GetKeyState(keyCode) == KeyState::KEY_UP;
	}

	bool GetKeyUp(int keyCode)
	{
		return GetKeyUp(static_cast<KeyCode>(keyCode));
	}

	bool GetKeyDown(KeyCode keyCode)
	{
		return GetKeyState(keyCode) == KeyState::KEY_DOWN;
	}

	bool GetKeyDown(int keyCode)
	{
		return GetKeyDown(static_cast<KeyCode>(keyCode));
	}

	bool GetKeyHold(KeyCode keyCode)
	{
		return GetKeyState(keyCode) == KeyState::KEY_HOLD;
	}

	bool GetKeyHold(int keyCode)
	{
		return GetKeyHold(static_cast<KeyCode>(keyCode));
	}

	////////////Loop Function////////////

	void DebugInputKey()
	{
		//Debug all key state
		int index = 0;
		for (auto keyCode : keyCodeEnumArr)
		{
			if (Input::GetKeyUp(keyCode))
			{
				test.setAnimFrame("Idle");
				std::cout << "GetKeyUp:       " << keyboardCodeName[index] << '\n';
				test.animUpdate();
				
			}
			if (Input::GetKeyHold(keyCode))
			{
				test.setAnimFrame("Running");
				std::cout << "GetKeyHold:     " << keyboardCodeName[index] << '\n';
				test.animUpdate();
				
			}
			if (Input::GetKeyDown(keyCode))
			{
				//test.setAnimFrame("Jumping");
				test.setAnimFrame("Dash");
				std::cout << "GetKeyDown:     " << keyboardCodeName[index] << '\n';
				test.animUpdate();
				
				
			}

			++index;
		}

		//Debug all mouse state
		index = 0;
		for (auto keyCode : mouseKeyCodeEnumArr)
		{
			if (Input::GetMouseUp(keyCode))
			{
				std::cout << "GetMouseUp:       " << mouseKeyCodeName[index] << '\n';
			}
			if (Input::GetMouseHold(keyCode))
			{
				std::cout << "GetMouseHold:     " << mouseKeyCodeName[index] << '\n';
			}
			if (Input::GetMouseDown(keyCode))
			{
				std::cout << "GetMouseDown:     " << mouseKeyCodeName[index] << '\n';
			}

			++index;
		}
	}

	void Init(void)
	{
		//Debug
		std::cout << "Initialize Input System" << std::endl;

	}

	void Update(void)
	{
		//Debug
		//std::cout << "Update Input System" << std::endl;

		//Update Input State
		UpdateAllInputState();

		//Update Mouse
		UpdateMousePosition();


		DebugInputKey();
	}

	void Terminate(void)
	{
		//Debug
		std::cout << "Terminate Input System" << std::endl;
	}




}