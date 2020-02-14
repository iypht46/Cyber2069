#include "AirFollowing.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

#include <iostream>

AirFollowing::AirFollowing()
{
	m_speed = 100.0f;
	rotAngle = 15.0f;
	rotRate = 1.5f;
}

void AirFollowing::SetPlayer(Transform* player) {
	m_target = player;
	t = &(m_gameObject->m_transform);
	rb = m_gameObject->GetComponent<Rigidbody>();

	if (m_gameObject == nullptr) {
		ENGINE_INFO("we're fine");
	}
	else {
		ENGINE_INFO("we're fucked");
	}

	if (rb == nullptr) {
		ENGINE_INFO("rbisfine");
	}
	else {
		ENGINE_INFO("rbisfucked");
	}

	if (t == nullptr) {
		ENGINE_INFO("tisfine");
	}
	else {
		ENGINE_INFO("tisfucked");
	}
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
	std::cout << myVel.x << endl;
	//ENGINE_INFO("Myvel {},{},{}", myVel.x, myVel.y, myVel.z);
	glm::vec3 direction = glm::normalize(m_target->GetPosition() - t->GetPosition());
	//ENGINE_INFO("direction {},{},{}", direction.x, direction.y, direction.z);
	float rot = glm::cross(glm::normalize(myVel), direction).z;
	//ENGINE_INFO("rot {}", rot);
	t->SetRotation(t->GetRotation() + (rot * rotRate * dt));
	//ENGINE_INFO("rotRate {}", rotRate);
	//ENGINE_INFO("dt {}", dt);
	//ENGINE_INFO("rotation {}", rot * rotRate * dt);
	ENGINE_INFO("================");
	rb->SetVelocity(glm::vec3(glm::cos(t->GetRotation() + this->rotAngle), glm::sin(t->GetRotation() + this->rotAngle), 0) * m_speed);

	if (direction.x > 0) {
		t->SetScale(glm::vec3(glm::abs(t->GetScale().x) * -1, t->GetScale().y, 1.0f));
	}
	else {
		t->SetScale(glm::vec3(glm::abs(t->GetScale().x), t->GetScale().y, 1.0f));;
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