#include "Transform.hpp"
#include "Core/Logger.hpp"
#include "Core/EC/GameObject.hpp"
#include "Graphic/Texture.hpp"
#include <iostream>
#include <memory>
using namespace glm;

float Transform::maxParallaxDistance = 5000.0f;

Transform::Transform() {
	m_position = vec3(0);
	m_localPosition = vec3(0);
	m_scale = vec3(1);
	m_localScale = vec3(1);
	m_rotation = 0;
	m_localRotation = 0;

	//parent is null

	Factory<Component, Transform>::Add(this);
}

Transform::~Transform() {
	Factory<Component, Transform>::Remove(this);
}

glm::vec3 Transform::GetPosition() {
	return m_position;
}

glm::vec3 Transform::GetLocalPosition() {
	return m_localPosition;
}

glm::vec3 Transform::GetScale() {
	return m_scale; //* m_meshScale;
}

glm::vec3 Transform::GetTrueScale() {
	return m_scale * m_meshScale;
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

float Transform::GetParallaxValue() {
	return glm::clamp(m_position.z / maxParallaxDistance, 0.0f, 1.0f);
}

glm::mat4 Transform::GetModelMatrix() {
	glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), radians(m_localRotation), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 sMat = glm::scale(glm::mat4(1.0f), m_localScale);
	glm::mat4 tMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_localPosition.x, m_localPosition.y, 0));
	glm::mat4 transformMat = tMat * rMat * sMat;
	if (!parent.expired()) {
		transformMat = parent.lock()->GetModelMatrix() * transformMat;
	}

	return transformMat;
}

void Transform::SetParent(std::weak_ptr<Transform> newParent) {
	parent = newParent;
	parent.lock()->children.push_back(weak_from_this());
}

bool Transform::HasParent()
{
	return !parent.expired();
}

Transform* Transform::GetChild(int index) {
	return (children.at(index)).lock().get();
}

Transform* Transform::GetParent() {
	if (parent.expired())
		return nullptr;

	return parent.lock().get();
}

unsigned int Transform::GetChildCount()
{
	return children.size();
}

/*
 *update world position according to parent rotation and local position
 */
void Transform::UpdateWorldPosition() {
	//update world position
	if (!parent.expired()) {
		float x = m_localPosition.x;
		float y = m_localPosition.y;
		float angle = radians(parent.lock()->GetRotation());

		vec3 worldPosDirection((x*cos(angle)) - (y*sin(angle)), (x*sin(angle)) + (y*cos(angle)), 0);

		m_position = parent.lock()->GetPosition() + worldPosDirection;
	}
	else {
		m_position = m_localPosition;
	}

	//update children position
	for (std::weak_ptr<Transform> child : children) {
		child.lock()->UpdateWorldPosition();
	}
}

/*
 *update world scale according to parent, local scale and local rotation
 */
void Transform::UpdateScale() {
	//update world scale
	if (!parent.expired()) {
		m_scale = parent.lock()->m_scale * m_localScale;
	}
	else {
		m_scale = m_localScale;
	}

	//update child scale
	for (std::weak_ptr<Transform> child : children) {
		child.lock()->UpdateScale();
	}
}

/**
*update world rotation according to parent and local rotation
*/
void Transform::UpdateRotation() {

	if (!parent.expired()) {
		m_rotation = m_localRotation + parent.lock()->GetRotation();
	}
	else {
		m_rotation = m_localRotation;
	}

	//update children rotation and position
	for (std::weak_ptr<Transform> child : children) {
		child.lock()->UpdateRotation();
		child.lock()->UpdateWorldPosition();
	}
}

void Transform::SetPosition(glm::vec3 position) {
	m_position = position;

	//update local position
	if (!parent.expired()) {
		vec3 relativePosition = m_position - parent.lock()->m_position;
		float revParentRotation = radians(-parent.lock()->GetRotation());

		m_localPosition = vec3(cos(revParentRotation)*relativePosition.x, sin(revParentRotation)*relativePosition.y, relativePosition.z);
	}
	else {
		m_localPosition = position;
	}

	//update children position
	for (std::weak_ptr<Transform> child : children) {
		child.lock()->UpdateWorldPosition();
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
	if (!parent.expired()) {
		m_localScale = m_scale / parent.lock()->m_scale;
	}
	else {
		m_localScale = m_scale;
	}

	//update child scale
	for (std::weak_ptr<Transform> child : children) {
		child.lock()->UpdateScale();
	}
}

void Transform::SetLocalScale(glm::vec3 localScale) {
	m_localScale = localScale;

	//update world scale and child scale
	UpdateScale();
}

void Transform::SetTrueScale(glm::vec3 trueScale) {
	m_scale = trueScale / m_meshScale;

	//update local scale
	if (!parent.expired()) {
		m_localScale = m_scale / parent.lock()->m_scale;
	}
	else {
		m_localScale = m_scale;
	}

	//update child scale
	for (std::weak_ptr<Transform> child : children) {
		child.lock()->UpdateScale();
	}
}

void Transform::SetRotation(float rotation) {

	m_rotation = rotation;

	//update local rotation
	if (!parent.expired()) {
		m_localRotation = m_rotation - parent.lock()->m_rotation;
	}
	else {
		m_localRotation = m_rotation;
	}

	//update children roataion and position
	for (std::weak_ptr<Transform> child : children) {
		child.lock()->UpdateRotation();
		child.lock()->UpdateWorldPosition();
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

void Transform::SetMeshScale(glm::vec3 meshScale)
{
	m_meshScale = meshScale;
}