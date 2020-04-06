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
	dashState = false;
	dashEnd = false;
	targetLocked = false;
}

void AirDash::Init() {

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
	glm::vec3 dir = m_target - self->GetPosition();
	float distance = glm::length(dir);
	m_angle = glm::atan(dir.y, dir.x);
	dashEnd = false;

	if (distance <= detectRange && !dashState) {
		timer -= dt;
		if (timer > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
			self->SetRotation(m_angle);
		}
		else {
			dashState = true;
			
		}
	}

	if (dashState) {
		if (distance > 50.0f) {
			rb->SetVelocity(glm::vec3(m_dashSpeed*glm::cos(self->GetRotation()), m_dashSpeed*glm::sin(self->GetRotation()), 0));
		}
		else 
		{
			Reset();
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

void AirDash::Reset() {
	timer = m_aimTime;
	dashState = false;
	dashEnd = true;
	targetLocked = false;
}


void AirDash::OnAwake() {
	this->m_target = glm::vec3(0);
	self = m_gameObject->m_transform.get();
	rb = m_gameObject->GetComponent<Rigidbody>();
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
