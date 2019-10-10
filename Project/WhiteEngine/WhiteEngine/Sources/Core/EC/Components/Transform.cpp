#include "Transform.hpp"

using namespace glm;

glm::vec3 Transform::GetPosition() {
	return m_position;
}		  

glm::vec3 Transform::GetScale() {
	return m_scale;
}

float Transform::GetRotation() {
	return m_rotation;
}

glm::vec3 Transform::Up() {
	return vec3(cos(radians(m_rotation + 90)), sin(radians(m_rotation + 90)), 0);
}

glm::vec3 Transform::Right() {
	return vec3(cos(radians(m_rotation)), sin(radians(m_rotation)), 0);
}

void Transform::SetPosition(glm::vec3& position) {
	if (parent != nullptr) {
		
	}
}

void Transform::Translate(glm::vec3& translation) {

}

void Transform::SetScale(glm::vec3 scale) {
	if (parent != nullptr) {

	}
}

void Transform::SetRotation(float rotation) {
	if (parent != nullptr) {

	}
}

void Transform::Rotate(float rotation) {

}

void Transform::OnAwake() {
}
void Transform::OnEnable() {
}
void Transform::OnStart() {
}
void Transform::OnUpdate(float dt) {
}
void Transform::OnFixedUpdate(float dt) {
}
void Transform::OnDisable() {
}