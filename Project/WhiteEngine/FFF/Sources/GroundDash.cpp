#include "GroundDash.hpp"
#include "Core/Logger.hpp"


GroundDash::GroundDash()
{

}


GroundDash::~GroundDash()
{

}

void GroundDash::Init() {
	m_dashSpeed = 700.0f;
	m_pauseTime = 1.0f;
	m_dashDistance = 25.0f;
	timer = m_pauseTime;
	dashing = false;
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
		dir = thisTransform->GetPosition().x - m_target->GetPosition().x;
		targetLocked = true;
	}
}

bool GroundDash::DashEnd() {
	return dashEnd;
}

void GroundDash::Dash(float dt) {
	float targetPosX = m_target->GetPosition().x;
	if (!dashing) {
		timer -= dt;
		if (timer > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
		}
		else {
			dashing = true;
		}
	}

	if (dashing) {
		if (dir < 0) {
			thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x), thisTransform->GetScale().y, 1.0f));
			rb->SetVelocity(glm::vec3(m_dashSpeed, 0, 0));
		}
		else{
			thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x) * -1, thisTransform->GetScale().y, 1.0f));
			rb->SetVelocity(glm::vec3(-m_dashSpeed, 0, 0));

		}
		
		if (dir <= 0 && (thisTransform->GetPosition().x >= targetPosX + m_dashDistance)) {
			timer = m_pauseTime;
			dashing = false;
			dashEnd = true;
			targetLocked = false;
		}
		else if (dir >= 0 && (thisTransform->GetPosition().x <= targetPosX - m_dashDistance)) {
			timer = m_pauseTime;
			dashing = false;
			dashEnd = true;
			targetLocked = false;
		}
	}



}

void GroundDash::OnStart() {

}

void GroundDash::OnUpdate(float dt) {

}

void GroundDash::OnFixedUpdate(float dt) {

}
