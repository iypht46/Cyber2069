#include "TargetArrow.h"

#include <glm/glm.hpp>

#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"

void TargetArrow::OnAwake() {
	m_gameObject->m_transform->SetLocalPosition(glm::vec3(distance, 0, 0));
}

void TargetArrow::OnFixedUpdate(float dt) {
	if (target != nullptr && orbit != nullptr) {

		m_gameObject->GetComponent<MeshRenderer>()->enabled = true;

		Transform* self = m_gameObject->m_transform.get();

		glm::vec2 dir = glm::normalize((glm::vec2)target->GetPosition() - (glm::vec2)orbit->GetPosition());

		float angle_rad = atan2(dir.y, dir.x);
		float angle_deg = glm::degrees(angle_rad);

		self->SetPosition(orbit->GetPosition() + glm::vec3(dir, 0) * distance);
		self->SetRotation(angle_deg);
	}
	else {
		m_gameObject->GetComponent<MeshRenderer>()->enabled = false;
	}
}