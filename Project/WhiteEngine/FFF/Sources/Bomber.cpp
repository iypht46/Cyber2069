#include "EnemyBehaviours.h"
#include "Core/Logger.hpp"

void Bomber::Init(std::shared_ptr<Transform> player) {
	SetTarget(player);
	airFollow = m_gameObject->GetComponent<AirFollowing>();
	airDash = m_gameObject->GetComponent<AirDash>();

	airFollow->SetPlayer(target);
	airDash->SetPlayer(target);

	targetDetectionRange = 1000.0f;
	DashTriggerRadius = 300.0f;

	rigidbody = m_gameObject->GetComponent<Rigidbody>();

	Enemy::Init();
}

void Bomber::OnStart() {

}

void Bomber::OnUpdate(float dt) {
	if (m_gameObject->Active()) {
		Enemy::OnUpdate(dt);

		if (glm::length(target->GetPosition() - m_gameObject->m_transform->GetPosition()) < DashTriggerRadius) {
			state = EnemyState::Active;
		}
		else if (foundTarget) {
			state = EnemyState::Chase;
		}
		else {
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
			break;
		default:
			break;
		}
	}
}