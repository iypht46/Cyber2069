#include "AirPatrol.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirPatrol::AirPatrol()
{
	m_speed = 30.0f;
	m_pointAX = 0.0f;
	m_pointBX = 0.0f;
}


AirPatrol::~AirPatrol()
{
	
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
	queen = &(m_gameObject->m_transform);
	rb = m_gameObject->GetComponent<Rigidbody>();
	m_pointAX = aX;
	m_pointBX = bX;

}

void AirPatrol::SetSpeed(float speed) {
	this->m_speed = speed;
}

void AirPatrol::OnAwake() {
	m_speed = 30.0f;
	m_pointAX = 0.0f;
	m_pointBX = 0.0f;
}

void AirPatrol::OnEnable() {

}

void AirPatrol::OnStart() {

}

void AirPatrol::OnUpdate(float dt) {

}

void AirPatrol::OnFixedUpdate(float dt) {

}

void AirPatrol::OnDisable() {

}
