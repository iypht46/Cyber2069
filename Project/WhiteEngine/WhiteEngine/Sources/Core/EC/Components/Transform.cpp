#include "Transform.hpp"

using namespace glm;

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

		m_position = parent->GetPosition() + length(relativePosition) * vec3(cos(radians(parentRotation + angleBetweenParent)), sin(radians(parentRotation + angleBetweenParent)), m_localPosition.z - parent->GetLocalPosition()));
	}
	else {
		m_position = m_localPosition;
	}

	//update child position
	//for (Transform* child : children) {
	//	child->UpdateWorldPosition();
	//}
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

	//update child roataion and position
	//for (Transform* child : children) {
	//	child->UpdateRotation();
	//	child->UpdateWorldPosition();
	//}
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

	//update child position
	//for (Transform* child : children) {
	//	child->UpdateWorldPosition();
	//}
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

	//update child roataion and position
	//for (Transform* child : children) {
	//	child->UpdateRotation();
	//	child->UpdateWorldPosition();
	//}
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