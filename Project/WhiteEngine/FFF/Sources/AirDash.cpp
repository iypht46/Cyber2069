#include "AirDash.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirDash::AirDash()
{
	m_dashSpeed = 700.0f;
	m_aimTime = 1.0f;
	m_aimSpeed = 50.0f;
	m_angle = 0.0f;
	timer = m_aimTime;
	dashing = false;
	dashEnd = false;
	targetLocked = false;
}

void AirDash::Init() {
	this->m_target = glm::vec3(0);
	bomber = &(m_gameObject->m_transform);
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
	
	float detectRange = 500.0f;
	glm::vec3 dir = m_target - bomber->GetPosition();
	float distance = glm::length(dir);
	m_angle = glm::atan(dir.y, dir.x);

	if (distance <= detectRange && !dashing) {
		timer -= dt;
		if (timer > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
			bomber->SetRotation(m_angle);
		}
		else {
			dashing = true;
			
		}
	}

	if (dashing) {
		if (distance > 50.0f) {
			rb->SetVelocity(glm::vec3(m_dashSpeed*glm::cos(bomber->GetRotation()), m_dashSpeed*glm::sin(bomber->GetRotation()), 0));
		}
		else 
		{
			timer = m_aimTime;
			dashing = false;
			dashEnd = true;
		}

	}
}

bool AirDash::DashEnd() {
	return dashEnd;
}

void AirDash::TargetLock(glm::vec3 pos) {
	if (!targetLocked) {
		this->m_target = pos;
		targetLocked = true;
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
