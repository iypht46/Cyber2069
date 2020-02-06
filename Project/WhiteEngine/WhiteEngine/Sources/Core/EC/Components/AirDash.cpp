#include "AirDash.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirDash::AirDash()
{
	m_dashSpeed = 200.0f;
	m_aimTime = 5.0f;
	m_aimSpeed = 50.0f;
}

void AirDash::SetPlayer(Transform* player) {
	this->m_target = player;
	bomber = &m_gameObject->m_transform;
	rb = m_gameObject->GetComponent<Rigidbody>();
}

void AirDash::SetDashSpeed(float value) {
	this->m_dashSpeed = value;
}

void AirDash::SetAimTime(float value) {
	this->m_aimTime = value;
}

void AirDash::SetAimSpeed(float value) {
	this->m_aimSpeed = value;
}

void AirDash::Dash(float dt) {

	//cout << "HELLO" << endl;
	float rangeX = m_target->GetPosition().x - bomber->GetPosition().x;
	//cout << "BYE" << endl;
	float rangeY = m_target->GetPosition().y - bomber->GetPosition().y;
	//cout << "DONE" << endl;

	if (rangeX <= 100.0 || rangeY <= 100.0) {
		m_aimTime -= dt;
		if (m_aimTime > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
			bomber->SetRotation(m_aimSpeed * dt);
		}
		else {
			rb->SetVelocity(glm::vec3(m_dashSpeed*glm::cos(bomber->GetRotation()), m_dashSpeed*glm::sin(bomber->GetRotation()), 0));
		}
	}
}

void AirDash::OnAwake() {

}

void AirDash::OnEnable() {

}

void AirDash::OnStart() {

}

void AirDash::OnUpdate(float dt) {

}

void AirDash::OnFixedUpdate(float dt) {

}

void AirDash::OnDisable() {

}
