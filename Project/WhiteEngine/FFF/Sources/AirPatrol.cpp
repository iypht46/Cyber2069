#include "AirPatrol.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirPatrol::AirPatrol()
{

}

void AirPatrol::OnAwake() {
	queen = GetGameObject()->m_transform.get();
	rb = GetGameObject()->GetComponent<Rigidbody>();
}

void AirPatrol::Patrol() {
	if (queen->GetPosition().x <= m_pointAX) {
		rb->SetVelocity(glm::vec3(m_speed, 0, 0));
		queen->SetScale(glm::vec3(glm::abs(queen->GetScale().x) * -1, queen->GetScale().y, 1.0f));
	}
	else if (queen->GetPosition().x >= m_pointBX) {
		rb->SetVelocity(glm::vec3(-m_speed, 0, 0));
		queen->SetScale(glm::vec3(glm::abs(queen->GetScale().x), queen->GetScale().y, 1.0f));
	}
}

void AirPatrol::SetPoint(float aX, float bX) {
	m_pointAX = aX;
	m_pointBX = bX;
}

void AirPatrol::SetSpeed(float speed) {
	this->m_speed = speed;
}
