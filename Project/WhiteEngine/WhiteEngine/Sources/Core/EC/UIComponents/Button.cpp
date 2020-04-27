#include "Button.hpp"

Button::Button() {
	
}

void Button::OnUpdate() {
	m_pos = Input::GetMouseWorldPosition();
	glm::vec2 diff = m_pos - (glm::vec2)m_gameObject->m_transform->GetPosition();
	if (glm::abs(diff.x) <= glm::abs((m_gameObject->m_transform->GetScale().x * 0.5f)) &&
		glm::abs(diff.y) <= glm::abs((m_gameObject->m_transform->GetScale().y * 0.5f))) {
		if (Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT)) {
			OnClick();
		}
	}
}
	

void Button::OnClick() {
	ENGINE_INFO("WHITE:FFF");
}

Button::~Button() {

}