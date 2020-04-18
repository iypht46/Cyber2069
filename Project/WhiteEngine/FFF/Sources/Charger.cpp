#include "EnemyBehaviours.h"

void Charger::OnAwake() {
	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();
	groundDash = m_gameObject->GetComponent<GroundDash>();

	rigidbody = m_gameObject->GetComponent<Rigidbody>();

	Enemy::OnAwake();
}

void Charger::OnUpdate(float dt) {
	Enemy::OnUpdate(dt);

	//if target in dash range
	if (state != EnemyState::Dash && foundTarget && glm::abs(target->GetPosition().y - m_gameObject->m_transform->GetPosition().y) <= DashTriggerRangeY) {
		groundDash->LockTarget(target);
		state = EnemyState::Dash;
	}
	else if(!foundTarget || glm::abs(target->GetPosition().y - m_gameObject->m_transform->GetPosition().y) >= DashTriggerRangeY){
		state = EnemyState::Idle;
	}
}

void Charger::OnFixedUpdate(float dt) {
	switch (state)
	{
	case Idle:
		groundPatrol->Patrol(dt);
		break;
	case Dash:
		groundDash->Dash(dt);
		if (groundDash->DashEnd()) {
			state = EnemyState::Idle;
		}
		break;
	default:
		break;
	}
}