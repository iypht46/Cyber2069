#include "EnemyBehaviours.h"

void Spitter::OnAwake() {
	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();
	shooting = m_gameObject->GetComponent<ObjectShooter>();
	rigidbody = m_gameObject->GetComponent<Rigidbody>();

	Enemy::OnAwake();
}

void Spitter::OnUpdate(float dt) {
	Enemy::OnUpdate(dt);

	//if target in range
	if (foundTarget) {
		state = EnemyState::Active;
	}
	else {
		state = EnemyState::Idle;
	}
}

void Spitter::OnFixedUpdate(float dt) {
	switch (state)
	{
	case EnemyState::Idle:
		//stay still
		//groundPatrol->Patrol(dt);
		break;
	case EnemyState::Active:
		shooting->Shoot(target);
		break;
	default:
		break;
	}
}