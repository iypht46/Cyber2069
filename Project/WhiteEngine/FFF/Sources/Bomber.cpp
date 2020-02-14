#include "EnemyBehaviours.h"
#include "Core/Logger.hpp"

void Bomber::Init(Transform* player) {
	SetTarget(player);
	airFollow = m_gameObject->GetComponent<AirFollowing>();
	airDash = m_gameObject->GetComponent<AirDash>();

	airFollow->SetPlayer(target);
	airDash->SetPlayer(target);

	targetDetectionRange = 1000.0f;
	DashTriggerRadius = 300.0f;

	rigidbody = m_gameObject->GetComponent<Rigidbody>();
}

void Bomber::OnStart() {

}

void Bomber::OnUpdate(float dt) {
	if (m_gameObject->Active()) {
		Enemy::OnUpdate(dt);

		if (glm::length(target->GetPosition() - m_gameObject->m_transform.GetPosition()) < DashTriggerRadius) {
			state = EnemyState::Active;
			ENGINE_INFO("Active");
		}
		else if (foundTarget) {
			state = EnemyState::Chase;
			ENGINE_INFO("chase");
		}
		else {
			state = EnemyState::Idle;
			ENGINE_INFO("Idle");
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