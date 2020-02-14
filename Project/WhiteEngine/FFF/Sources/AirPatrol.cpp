#include "AirPatrol.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirPatrol::AirPatrol()
{
	m_speed = 75.0f;
	queen = &m_gameObject->m_transform;
	rb = m_gameObject->GetComponent<Rigidbody>();
}


AirPatrol::~AirPatrol()
{
	
}

void AirPatrol::Patrol(float dt) {
	if (queen->GetPosition() == m_pointA->GetPosition()) {
		rb->SetVelocity(glm::vec3(m_speed * dt, 0, 0));
	}
	else if (queen->GetPosition() == m_pointB->GetPosition()) {
		rb->SetVelocity(glm::vec3(-m_speed * dt, 0, 0));
	}
}

void AirPatrol::SetPoint(float aX, float aY, float bX, float bY) {
	m_pointA->SetPosition(glm::vec3(aX, aY, 0));
	m_pointB->SetPosition(glm::vec3(bX, bY, 0));
}

void AirPatrol::OnAwake() {

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
