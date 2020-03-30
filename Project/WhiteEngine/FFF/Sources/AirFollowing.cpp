#include "AirFollowing.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirFollowing::AirFollowing()
{
	m_speed = 300.0f;
	rotAngle = 15.0f;
	rotRate = 2.0f;
}

void AirFollowing::SetPlayer(Transform* player) {
	m_target = player;
	t = &(m_gameObject->m_transform);
	rb = m_gameObject->GetComponent<Rigidbody>();
}

void AirFollowing::SetFlySpeed(float value) {
	m_speed = value;
}

void AirFollowing::SetRotAngle(float value) {
	rotAngle = value;
}

void AirFollowing::SetRotRate(float value) {
	rotRate = value;
}

void AirFollowing::FollowPlayer(float dt) {
	glm::vec3 myVel = rb->GetVelocity();
	glm::vec3 direction = glm::normalize(m_target->GetPosition() - t->GetPosition());
	float rot = glm::cross(glm::normalize(myVel), direction).z;
	t->SetRotation(t->GetRotation() + (rot * rotRate * dt));

	rb->SetVelocity(glm::vec3(glm::cos(t->GetRotation() + this->rotAngle), glm::sin(t->GetRotation() + this->rotAngle), 0) * m_speed);

	if (direction.x > 0) {
		if (facingRight) 
		{
			flip();
		}
	}
	else {
		if (!facingRight)
		{
			flip();
		}
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