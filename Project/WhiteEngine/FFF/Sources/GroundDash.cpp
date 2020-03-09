#include "GroundDash.hpp"



GroundDash::GroundDash()
{

}


GroundDash::~GroundDash()
{

}

void GroundDash::Init() {
	m_dashSpeed = 700.0f;
	m_pauseTime = 1.0f;
	m_dashDistance = 50.0f;
	timer = m_pauseTime;
	dashState = false;
	dashEnd = false;
	targetLocked = false;
	m_target = nullptr;
	thisTransform = &(m_gameObject->m_transform);
	rb = m_gameObject->GetComponent<Rigidbody>();
}

void GroundDash::SetDashSpeed(float val) {
	this->m_dashSpeed = val;
}

void GroundDash::SetDashDis(float val) {
	this->m_dashDistance = val;
}

void GroundDash::SetPauseTime(float val) {
	this->m_pauseTime = val;
}

void GroundDash::LockTarget(Transform* target) {
	if (!targetLocked) {
		this->m_target = target;
		targetLocked = true;
	}
}

bool GroundDash::DashEnd() {
	return dashEnd;
}

void GroundDash::Dash(float dt) {
	float dir = thisTransform->GetPosition().x - m_target->GetPosition().x;
	if (!dashState) {
		timer -= dt;
		if (timer > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
		}
		else {
			dashState = true;
		}
	}

	if (dashState) {
		if (dir < 0) {
			thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x), thisTransform->GetScale().y, 1.0f));
			rb->SetVelocity(glm::vec3(m_dashSpeed, 0, 0));
		}
		else {
			thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x) * -1, thisTransform->GetScale().y, 1.0f));
			rb->SetVelocity(glm::vec3(-m_dashSpeed, 0, 0));
		}
		timer = m_pauseTime;
		dashState = false;
		dashEnd = true;
	}

}

void GroundDash::OnStart() {

}

void GroundDash::OnUpdate(float dt) {

}

void GroundDash::OnFixedUpdate(float dt) {

}
