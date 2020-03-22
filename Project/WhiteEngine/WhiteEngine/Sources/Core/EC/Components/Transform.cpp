#include "Transform.hpp"
#include "../../Logger.hpp"
#include <iostream>
#include <memory>
using namespace glm;

Transform::Transform() {
	m_position = vec3(0, 0, 0);
	ENGINE_INFO("{}", m_position.x);
	m_scale = vec3(1, 1, 1);
	m_rotation = 0;
	m_localRotation = 0;

	parent = nullptr;

	//Factory<Transform>::Add(this);
}

glm::vec3 Transform::GetPosition() {
	return m_position;
}		  

glm::vec3 Transform::GetLocalPosition() {
	return m_localPosition;
}

glm::vec3 Transform::GetScale() {
	return m_scale;
}

float Transform::GetRotation() {
	return m_rotation;
}

float Transform::GetLocalRotation() {
	return m_localRotation;
}

glm::vec3 Transform::Up() {
	return vec3(cos(radians(m_rotation + 90)), sin(radians(m_rotation + 90)), 0);
}

glm::vec3 Transform::Right() {
	return vec3(cos(radians(m_rotation)), sin(radians(m_rotation)), 0);
}

glm::mat4 Transform::GetModelMatrix() {
	glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), radians(m_localRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 sMat = glm::scale(glm::mat4(1.0f), m_localScale);
	glm::mat4 tMat = glm::translate(glm::mat4(1.0f), m_localPosition);
	glm::mat4 transformMat = tMat * rMat * sMat;

	if (parent != nullptr) {
		transformMat = parent->GetModelMatrix() * transformMat;
	}

	return transformMat;
}

void Transform::SetParent(std::shared_ptr<Transform> newParent) {
	parent = newParent;
	parent->children.push_back(std::shared_ptr<Transform>(this));
}

Transform* Transform::GetChild(int index) {
	return (children.at(index)).get();
}

/*
 *update world position according to parent rotation and local position
 */
void Transform::UpdateWorldPosition() {
	//update world position
	if (parent != nullptr) {
		float x = m_localPosition.x;
		float y = m_localPosition.y;
		float angle = radians(parent->GetRotation());

		vec3 worldPosDirection((x*cos(angle)) - (y*sin(angle)), (x*sin(angle)) + (y*cos(angle)), 0);

		m_position = parent->GetPosition() + worldPosDirection;
	}
	else {
		m_position = m_localPosition;
	}

	//update children position
	for (std::shared_ptr<Transform> child : children) {
		child->UpdateWorldPosition();
	}
}

/*
 *update world scale according to parent, local scale and local rotation
 */
void Transform::UpdateScale() {
	//update world scale
	if (parent != nullptr) {
		m_scale = parent->m_scale * m_localScale;
	}
	else {
		m_scale = m_localScale;
	}

	//update child scale
	for (std::shared_ptr<Transform> child : children) {
		child->UpdateScale();
	}
}

/**
*update world rotation according to parent and local rotation
*/
void Transform::UpdateRotation() {

	if (parent != nullptr) {
		m_rotation = m_localRotation + parent->GetRotation();
	}
	else {
		m_rotation = m_localRotation;
	}

	//update children rotation and position
	for (std::shared_ptr<Transform> child : children) {
		child->UpdateRotation();
		child->UpdateWorldPosition();
	}
}

void Transform::SetPosition(glm::vec3 position) {
	m_position = position;

	//update local position
	if (parent != nullptr) {
		vec3 relativePosition = m_position - parent->m_position;
		float revParentRotation = radians(-parent->GetRotation());

		m_localPosition = vec3(cos(revParentRotation)*relativePosition.x, sin(revParentRotation)*relativePosition.y, relativePosition.z);
	}
	else {
		m_localPosition = position;
	}

	//update children position
	for (std::shared_ptr<Transform> child : children) {
		child->UpdateWorldPosition();
	}
}

void Transform::SetLocalPosition(glm::vec3 localposition) {
	m_localPosition = localposition;
	
	//update world position and child position
	UpdateWorldPosition();
}

void Transform::Translate(glm::vec3 translation) {
	SetPosition(m_position + translation);
}

void Transform::SetScale(glm::vec3 scale) {

	m_scale = scale;

	//update local scale
	if (parent != nullptr) {
		m_localScale = m_scale / parent->m_scale;
	}
	else {
		m_localScale = scale;
	}

	//update child scale
	for (std::shared_ptr<Transform> child : children) {
		child->UpdateScale();
	}
}

void Transform::SetLocalScale(glm::vec3 localScale) {
	m_localScale = localScale;

	//update world scale and child scale
	UpdateScale();
}

void Transform::SetRotation(float rotation) {

	m_rotation = rotation;

	//update local rotation
	if (parent != nullptr) {
		m_localRotation = m_rotation - parent->m_rotation;
	}
	else {
		m_localRotation = m_rotation;
	}

	//update children roataion and position
	for (std::shared_ptr<Transform> child : children) {
		child->UpdateRotation();
		child->UpdateWorldPosition();
	}
}

void Transform::SetLocalRotation(float localrotation) {

	m_localRotation = localrotation;

	//update world rotation
	UpdateRotation();
}

void Transform::Rotate(float rotation) {
	SetRotation(m_rotation + rotation);
}