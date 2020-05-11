#include "EnemyBehaviours.h"
#include "Weapon.hpp"

void Spitter::OnAwake() {
	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();
	shooting = m_gameObject->GetComponent<ObjectShooter>();
	rigidbody = m_gameObject->GetComponent<Rigidbody>();

	Enemy::OnAwake();
}

void Spitter::SetStats(float Speed, float HP, float FireRate) {
	groundPatrol->SetSpeed(Speed);
	hpSystem->SetMaxHP(HP);
	shooting->firerate = FireRate;
}


void Spitter::OnUpdate(float dt) {
	Enemy::OnUpdate(dt);

	//if target in range
	if (foundTarget) {
		state = EnemyState::Active;
		animator->setNextState(0);
	}
	else {
		state = EnemyState::Idle;
		animator->setNextState(0);
	}
}

void Spitter::OnFixedUpdate(float dt) {
	switch (state)
	{
	case EnemyState::Idle:
		groundPatrol->Patrol(dt);
		break;
	case EnemyState::Active:
		if (shooting->CooledDown()) {
			animator->setCurrentState(1);
			Spit();
		}
		break;
	default:
		break;
	}
}

void Spitter::Spit() {
	GameObject* bullet = shooting->Shoot(target);
	if (bullet != nullptr) {
		bullet->GetComponent<FlakBullet>()->Destination = (glm::vec2)target->GetPosition();
	}
}