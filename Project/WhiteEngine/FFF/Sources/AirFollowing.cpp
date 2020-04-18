#include "AirFollowing.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirFollowing::AirFollowing()
{

}

void AirFollowing::OnAwake() {
	t = GetGameObject()->m_transform.get();
	rb = GetGameObject()->GetComponent<Rigidbody>();
	e = GetGameObject()->GetComponent<Character>();
}

void AirFollowing::SetPlayer(Transform* player) {
	m_target = player;
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
		if (e->facingRight)
		{
			e->flip();
		}
	}
	else {
		if (!e->facingRight)
		{
			e->flip();
		}
	}

}