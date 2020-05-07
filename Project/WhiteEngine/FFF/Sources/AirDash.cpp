#include "AirDash.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"
#include "PlayerController.hpp"

AirDash::AirDash()
{

}

void AirDash::OnAwake() {
	this->m_target = glm::vec3(0);
	self = m_gameObject->m_transform.get();
	rb = m_gameObject->GetComponent<Rigidbody>();

	Reset();
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
	
	glm::vec3 dir = m_target - self->GetPosition();
	float distance = glm::length(dir);
	m_angle = glm::atan(dir.y, dir.x);
	dashEnd = false;

	if (!dashState) {
		timer -= dt;
		if (timer > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
			self->SetRotation(m_angle);
		}
		else {
			dashState = true;
			
		}
	}else if (dashState) {
		if (distance <= 10.0f) {
			Reset();
		}
		else 
		{
			rb->SetVelocity(glm::vec3(m_dashSpeed * glm::cos(self->GetRotation()), m_dashSpeed * glm::sin(self->GetRotation()), 0));
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

void AirDash::OnTriggerEnter(const Physic::Collision collision) {
	GameObject* obj = collision.m_otherCollider->GetGameObject();
	PlayerController* player = obj->GetComponent<PlayerController>();

	///damage player
	if (player != nullptr) {
		obj->GetComponent<HPsystem>()->TakeDamage(dashDamage);
	}
}