#include "EnemyBehaviours.h"

void Tank::OnAwake() {
	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();
	rigidbody = m_gameObject->GetComponent<Rigidbody>();

	Enemy::OnAwake();
}

void Tank::SetStats(float Speed, float HP) {
	groundPatrol->SetSpeed(Speed);
	hpSystem->SetMaxHP(HP);
}

void Tank::OnUpdate(float dt) {
	Enemy::OnUpdate(dt);

	state = EnemyState::Idle;
}

void Tank::OnFixedUpdate(float dt) {
	switch (state)
	{
	case Idle:
		groundPatrol->Patrol(dt);
		break;
	default:
		break;
	}
}