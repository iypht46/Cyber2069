#include "EnemyBehaviours.h"
#include "Core/Logger.hpp"

void Bomber::Init(Transform* player) {
	SetTarget(player);
	airFollow = m_gameObject->GetComponent<AirFollowing>();
	airDash = m_gameObject->GetComponent<AirDash>();
	explosion = m_gameObject->AddComponent<Explosion>();

	airFollow->SetPlayer(target);
	airDash->Init();
	explosion->Init();

	targetDetectionRange = 1000.0f;
	DashTriggerRadius = 300.0f;
	ExplodeTriggerRadius = 100.0f;

	rigidbody = m_gameObject->GetComponent<Rigidbody>();

	Enemy::Init();
}

void Bomber::OnStart() {

}

void Bomber::OnUpdate(float dt) {
	if (m_gameObject->Active()) {
		Enemy::OnUpdate(dt);

		if (glm::length(target->GetPosition() - m_gameObject->m_transform.GetPosition()) < DashTriggerRadius) {
			airDash->TargetLock(target->GetPosition());
			state = EnemyState::Active;
			
		}
		else if (foundTarget && state != EnemyState::Active) {
			state = EnemyState::Chase;
		}
		else if(state != EnemyState::Active){
			state = EnemyState::Idle;
		}

	}
}

void Bomber::OnFixedUpdate(float dt) {
	if (m_gameObject->Active()) {
		switch (state)
		{
		case EnemyState::Idle:
			rigidbody->SetVelocity(glm::vec3(0));
			break;
		case EnemyState::Chase:
			airFollow->FollowPlayer(dt);
			break;
		case EnemyState::Active:
			airDash->Dash(dt);
			if (airDash->DashEnd()) {
				explosion->Explode();
				hpSystem->Dead();

			}
			break;
		default:
			break;
		}
	}
}