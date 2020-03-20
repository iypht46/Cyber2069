#include "EnemyBehaviours.h"

void Charger::Init(Transform* player) {
	SetTarget(player);

	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();
	groundDash = m_gameObject->GetComponent<GroundDash>();

	groundPatrol->Init();
	groundDash->Init();
	targetDetectionRange = 200.0f;
	groundPatrol->SetMaxDelay(0.5f);

	rigidbody = m_gameObject->GetComponent<Rigidbody>();
	rigidbody->SetGravityScale(0);

	Enemy::Init();
}

void Charger::OnStart() {

}

void Charger::OnUpdate(float dt) {
	if (m_gameObject->Active()) {
		Enemy::OnUpdate(dt);

		if (glm::length(target->GetPosition() - m_gameObject->m_transform.GetPosition()) < targetDetectionRange && target->GetPosition().y <= m_gameObject->m_transform.GetPosition().y) {
			groundDash->LockTarget(target);
			state = EnemyState::Dash;

		}
		else if (foundTarget && state != EnemyState::Dash) {
			state = EnemyState::Chase;
		}
		else {
			state = EnemyState::Idle;
		}
	}
}

void Charger::OnFixedUpdate(float dt) {
	if (m_gameObject->Active()) {
		switch (state)
		{
		case Idle:
			groundPatrol->Patrol(dt);
			break;
		case Chase:
			groundPatrol->Patrol(dt);
			break;
		case Active:
		case Dash:
			groundDash->Dash(dt);
			if (groundDash->DashEnd()) {
				state = EnemyState::Chase;
			}
			break;
		case Reset:
		default:
			break;
		}
	}
}