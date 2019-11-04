#pragma once

#include "Graphic/Window.hpp"
#include <glm/glm.hpp>
#include <iostream>

namespace Input
{
	
	////////////Enum Input Key////////////
	enum class KeyCode : int
	{
		KEY_INVALID = -1,
		KEY_A = GLFW_KEY_A,
		KEY_B = GLFW_KEY_B,
		KEY_C = GLFW_KEY_C,
		KEY_D = GLFW_KEY_D,
		KEY_E = GLFW_KEY_E,
		KEY_F = GLFW_KEY_F,
		KEY_G = GLFW_KEY_G,
		KEY_H = GLFW_KEY_H,
		KEY_I = GLFW_KEY_I,
		KEY_J = GLFW_KEY_J,
		KEY_K = GLFW_KEY_K,
		KEY_L = GLFW_KEY_L,
		KEY_M = GLFW_KEY_M,
		KEY_N = GLFW_KEY_N,
		KEY_O = GLFW_KEY_O,
		KEY_P = GLFW_KEY_P,
		KEY_Q = GLFW_KEY_Q,
		KEY_R = GLFW_KEY_R,
		KEY_S = GLFW_KEY_S,
		KEY_T = GLFW_KEY_T,
		KEY_U = GLFW_KEY_U,
		KEY_V = GLFW_KEY_V,
		KEY_W = GLFW_KEY_W,
		KEY_X = GLFW_KEY_X,
		KEY_Y = GLFW_KEY_Y,
		KEY_Z = GLFW_KEY_Z,

		KEY_0 = GLFW_KEY_0,
		KEY_1 = GLFW_KEY_1,
		KEY_2 = GLFW_KEY_2,
		KEY_3 = GLFW_KEY_3,
		KEY_4 = GLFW_KEY_4,
		KEY_5 = GLFW_KEY_5,
		KEY_6 = GLFW_KEY_6,
		KEY_7 = GLFW_KEY_7,
		KEY_8 = GLFW_KEY_8,
		KEY_9 = GLFW_KEY_9,

		KEY_SPACE = GLFW_KEY_SPACE,
		KEY_ENTER = GLFW_KEY_ENTER,

		KEY_RIGHT = GLFW_KEY_RIGHT,
		KEY_LEFT = GLFW_KEY_LEFT,
		KEY_DOWN = GLFW_KEY_DOWN,
		KEY_UP = GLFW_KEY_UP,

		KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
		KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
		KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT,
		KEY_ESCAPE = GLFW_KEY_ESCAPE,
		KEY_COUNT
	};

	enum class MouseKeyCode : unsigned
	{
		MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
		MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
		MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
	};

	enum class KeyState : unsigned
	{
		KEY_NONE = 0,
		KEY_UP,
		KEY_DOWN,
		KEY_HOLD
	};

	////////////Mouse Interface////////////
	bool GetMouseUp(MouseKeyCode);
	bool GetMouseUp(unsigned);

	bool GetMouseDown(MouseKeyCode);
	bool GetMouseDown(unsigned);

	bool GetMouseHold(MouseKeyCode);
	bool GetMouseHold(unsigned);

	glm::vec2 GetMouseScreenPosition();
	glm::vec2 GetMouseWorldPosition();
	glm::vec2 GetMouseOffset();

	KeyState GetMouseState(MouseKeyCode);

	////////////Key Interface////////////
	bool GetKeyUp(KeyCode);
	bool GetKeyUp(int);

	bool GetKeyDown(KeyCode);
	bool GetKeyDown(int);

	bool GetKeyHold(KeyCode);
	bool GetKeyHold(int);

	KeyState GetKeyState(KeyCode);
	////////////System Interface////////////
	void Init(bool);
	void Update(void);
	void Terminate(void);

}