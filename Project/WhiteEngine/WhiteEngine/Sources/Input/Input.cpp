//Standard Header
#include <unordered_map>

//System Headers
#include "Input/Input.hpp"
#include "Graphic/Window.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"

using namespace Graphic;
using namespace glm;

namespace Input
{
	//Variables
	vec2 g_mousePosition;
	vec2 g_prevMousePosition;
	bool g_inputDebug;

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
	glm::vec2 GetMouseScreenPosition() 
	{
		float pos[2];

		pos[0] = g_mousePosition.x - Window::GetWidth() / 2;
		pos[1] = 0.0f - (g_mousePosition.y - Window::GetHeight() / 2);

		glm::vec2 mouse_pos = glm::vec2(pos[0], pos[1]);

		return mouse_pos;
	}


	glm::vec2 GetMouseWorldPosition()
	{
		glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 dirUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::mat4 viewMat = glm::lookAt(position, position + dir, dirUp);

		glm::mat4 projMat = viewMat * Graphic::getCamera()->GetProjectionMatrix();
		glm::mat4 matInverse = glm::inverse(projMat);


		float in[4];
		float winZ = 1.0;


		in[0] = (2.0f * ((float)(g_mousePosition.x - 0) / (Window::GetWidth() - 0))) - 1.0f;
		in[1] = 1.0f - (2.0f * ((float)(g_mousePosition.y - 0) / (Window::GetHeight() - 0)));
		in[2] = 2.0 * winZ - 1.0;
		in[3] = 1.0;

		glm::vec4 vIn = glm::vec4(in[0], in[1], in[2], in[3]);
		glm::vec4 pos = vIn * matInverse;

		pos.w = 1.0 / pos.w;

		pos.x *= pos.w;
		pos.y *= pos.w;
		pos.z *= pos.w;

		glm::vec2 mouse_pos = glm::vec2(pos.x + Graphic::getCamera()->GetCampos().x, pos.y + Graphic::getCamera()->GetCampos().y);

		return mouse_pos;
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
				ENGINE_INFO("GetKeyUp  :       {}", keyboardCodeName[index]);
			}
			if (Input::GetKeyHold(keyCode))
			{
				ENGINE_INFO("GetKeyHold:       {}", keyboardCodeName[index]);
			}
			if (Input::GetKeyDown(keyCode))
			{
				ENGINE_INFO("GetKeyDown:       {}", keyboardCodeName[index]);
			}

			++index;
		}

		//Debug all mouse state
		index = 0;
		for (auto keyCode : mouseKeyCodeEnumArr)
		{
			if (Input::GetMouseUp(keyCode))
			{
				ENGINE_INFO("GetMouseUp  :       {}", mouseKeyCodeName[index]);
			}
			if (Input::GetMouseHold(keyCode))
			{
				ENGINE_INFO("GetMouseHold:       {}", mouseKeyCodeName[index]);
			}
			if (Input::GetMouseDown(keyCode))
			{
				ENGINE_INFO("GetMouseDown:       {}", mouseKeyCodeName[index]);
			}

			++index;
		}
	}

	void Init(bool isDebug)
	{
		//Debug
		g_inputDebug = isDebug;
		ENGINE_WARN("Input System Initialized");
	}

	void Update(void)
	{
		//Debug
		//std::cout << "Update Input System" << std::endl;

		//Update Input State
		UpdateAllInputState();

		//Update Mouse
		UpdateMousePosition();

		if (g_inputDebug)
		{
			DebugInputKey();
		}
		
	}

	void Terminate(void)
	{
		//Debug
		ENGINE_WARN("Input System Terminated");
	}




}
