#include "AirFollowing.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirFollowing::AirFollowing()
{
	m_speed = 100.0f;
	rotAngle = 15.0f;
	rotRate = 1.5f;
}

void AirFollowing::SetPlayer(Transform& player) {
	this->m_target = &player;
	flyer = &m_gameObject->m_transform;
}

void AirFollowing::SetFlySpeed(float value) {
	this->m_speed = value;
}

void AirFollowing::SetRotAngle(float value) {
	this->rotAngle = value;
}

void AirFollowing::SetRotRate(float value) {
	this->rotRate = value;
}

void AirFollowing::FollowPlayer(float dt) {
	
	Rigidbody* rb = m_gameObject->GetComponent<Rigidbody>();
	glm::vec3 myVel = m_gameObject->GetComponent<Rigidbody>()->GetVelocity();
	glm::vec3 direction = glm::normalize(m_target->GetPosition() - flyer->GetPosition());
	float rot = glm::cross(glm::normalize(myVel), direction).z;
	flyer->SetRotation(flyer->GetRotation() + (rot * this->rotRate * dt));

	rb->SetVelocity(glm::vec3(m_speed * glm::cos(flyer->GetRotation() + this->rotAngle),
		m_speed * glm::sin(flyer->GetRotation() + this->rotAngle), 0));

	if (direction.x > 0) {
		flyer->SetScale(glm::vec3(glm::abs(flyer->GetScale().x) * -1, flyer->GetScale().y, 1.0f));

	}
	else {
		flyer->SetScale(glm::vec3(glm::abs(flyer->GetScale().x), flyer->GetScale().y, 1.0f));;
	}
}

void AirFollowing::OnAwake() {

}

void AirFollowing::OnEnable() {

}

void AirFollowing::OnStart() {

}

void AirFollowing::OnUpdate(float dt) {

}

void AirFollowing::OnFixedUpdate(float dt) {

}

void AirFollowing::OnDisable() {

}