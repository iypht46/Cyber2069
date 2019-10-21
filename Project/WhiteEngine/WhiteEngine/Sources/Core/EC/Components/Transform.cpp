#include "Transform.hpp"

using namespace glm;

Transform::Transform() {
	m_position = vec3(0, 0, 0);
	m_scale = vec3(1, 1, 1);
	m_rotation = 0;

	parent = nullptr;
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
	//transformation
	/*mat4 transformMatrices(1);
	transformMatrices[0].x = m_position.x;
	transformMatrices[0].y = m_position.y;
	transformMatrices[0].z = m_position.z;

	//rotation
	mat4 RotationMatrices(1);
	RotationMatrices[0][0] = cos(radians(m_rotation));
	RotationMatrices[0][1] = -sin(radians(m_rotation));
	RotationMatrices[1][0] = sin(radians(m_rotation));
	RotationMatrices[1][1] = cos(radians(m_rotation));

	//scale
	mat4 ScaleMatrices(1);
	transformMatrices[0].x = m_scale.x;
	transformMatrices[1].y = m_scale.y;
	transformMatrices[2].z = m_scale.z;

	return mat4(1.0f) * ScaleMatrices * RotationMatrices * transformMatrices;*/

	glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 sMat = glm::scale(glm::mat4(1.0f), m_scale);
	glm::mat4 tMat = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 transform = tMat * sMat * rMat;

	return transform;
}

/**
 *update world position according to parent rotation and local position
 */
void Transform::UpdateWorldPosition() {
	//update world position
	if (parent != nullptr) {
		vec3 relativePosition = m_position - parent->m_position;
		float angleBetweenParent = degrees(atan2(relativePosition.y, relativePosition.x));
		float parentRotation = radians(parent->GetRotation());

		vec3 worldPosDirection(cos(radians(parentRotation + angleBetweenParent)), sin(radians(parentRotation + angleBetweenParent)), m_localPosition.z - parent->GetPosition().z);
		worldPosDirection /= worldPosDirection.length();
		m_position = parent->GetPosition() + (length(relativePosition) * worldPosDirection);
	}
	else {
		m_position = m_localPosition;
	}

	//update children position
	for (Transform* child : children) {
		child->UpdateWorldPosition();
	}
}

///**
// *update world scale according to parent, local scale and local rotation
// */
//void Transform::UpdateScale() {
//	//update world scale
//	if (parent != nullptr) {
//
//	}
//	else {
//		m_scale = m_localScale;
//	}
//
//	//update child scale
//
//}

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
	for (Transform* child : children) {
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
	for (Transform* child : children) {
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

	////update local scale
	//if (parent != nullptr) {

	//}

	//update child scale
	//for (Transform* child : children) {
	//	child->UpdateWorldPosition();
	//}
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
	for (Transform* child : children) {
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