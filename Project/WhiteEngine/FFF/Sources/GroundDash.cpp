#include "GroundDash.hpp"
#include "Core/Logger.hpp"


GroundDash::GroundDash()
{

}


GroundDash::~GroundDash()
{

}


void GroundDash::OnAwake() {
	pauseTimer = m_pauseTime;
	dashing = false;
	dashEnd = false;
	targetLocked = false;
	m_target = nullptr;
	thisTransform = m_gameObject->m_transform.get();
	rb = m_gameObject->GetComponent<Rigidbody>();
	patrol = m_gameObject->GetComponent<GroundPatrol>();
}

void GroundDash::SetDashSpeed(float val) {
	this->m_dashSpeed = val;
}

void GroundDash::SetDashDis(float val) {
	this->m_exceedingDashDistance = val;
}

void GroundDash::SetPauseTime(float val) {
	this->m_pauseTime = val;
}

void GroundDash::LockTarget(Transform* target) {
	if (!targetLocked) {
		this->m_target = target;
		targetLocked = true;
		dashEnd = false;


		dashDestinationX = m_target->GetPosition().x;
		if (glm::abs(dashDestinationX - thisTransform->GetPosition().x) < m_minDashDistance) {
			dashDestinationX = thisTransform->GetPosition().x + (glm::sign(dashDestinationX - thisTransform->GetPosition().x) * m_minDashDistance);
		}

		dashDestinationX += glm::sign(dashDestinationX) > 0 ? m_exceedingDashDistance : -m_exceedingDashDistance;
	}
}

bool GroundDash::DashEnd() {
	return dashEnd;
}

void GroundDash::Dash(float dt) {

	//if not dashing, wait until timer end then begin dashing
	if (!dashing && !dashEnd) {
		pauseTimer -= dt;
		if (pauseTimer > 0) {
			rb->SetVelocity(glm::vec3(0, rb->GetVelocity().y, 0));
		}
		else {
			dashing = true;
		}
	}
	else if (dashing && !dashEnd) {
		thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x) * glm::sign(dashDestinationX - thisTransform->GetPosition().x), thisTransform->GetScale().y, 1.0f));
		rb->SetVelocity(glm::vec3(m_dashSpeed * glm::sign(dashDestinationX - thisTransform->GetPosition().x), rb->GetVelocity().y, 0));

		//if close to stopping destination or at cliff, stop dashing and reset dash
		if (glm::abs(dashDestinationX - thisTransform->GetPosition().x) < 50.0f || !patrol->CheckGroundPath()) {
			pauseTimer = m_pauseTime;
			dashing = false;
			dashEnd = true;
			targetLocked = false;
		}
	}
}
