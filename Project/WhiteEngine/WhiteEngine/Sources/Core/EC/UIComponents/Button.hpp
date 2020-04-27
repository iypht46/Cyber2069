#pragma once
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Component.hpp"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"


class Button : public Component {
private:
	glm::vec2 m_pos;
public:
	Button();
	void OnUpdate();
	void OnClick();
	~Button();
};