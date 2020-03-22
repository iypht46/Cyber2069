#include "AirDash.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirDash::AirDash()
{
	m_dashSpeed = 700.0f;
	m_aimTime = 1.0f;
	m_aimSpeed = 50.0f;
	m_angle = 0.0f;
	maxExplodeTime = 0.8f;
	m_explodeCountDown = maxExplodeTime;
	timer = m_aimTime;
	dashState = false;
}

void AirDash::SetPlayer(Transform* player) {
	this->m_target = player;
	bomber = GetGameObject()->m_transform.get();
	rb = GetGameObject()->GetComponent<Rigidbody>();
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
	
	float detectRange = 500.0f;
	glm::vec3 dir = m_target->GetPosition() - bomber->GetPosition();
	float distance = glm::length(dir);
	/*float rangeX = m_target->GetPosition().x - bomber->GetPosition().x;
	float rangeY = m_target->GetPosition().y - bomber->GetPosition().y;*/
	m_angle = glm::atan(dir.y, dir.x);

	if (distance <= detectRange && !dashState) {
		timer -= dt;
		if (timer > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
			bomber->SetRotation(m_angle);
		}
		else {
			dashState = true;
			
		}
	}

	if (dashState) {
		m_explodeCountDown -= dt;
		if (distance > 10.0f && m_explodeCountDown > 0) {
			rb->SetVelocity(glm::vec3(m_dashSpeed*glm::cos(bomber->GetRotation()), m_dashSpeed*glm::sin(bomber->GetRotation()), 0));
		}
		else 
		{
			m_explodeCountDown = maxExplodeTime;
			timer = m_aimTime;
			dashState = false;
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
