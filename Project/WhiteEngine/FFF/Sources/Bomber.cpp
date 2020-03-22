#include "EnemyBehaviours.h"
#include "Core/Logger.hpp"

void Bomber::Init(Transform* player) {
	SetTarget(player);
	airFollow = GetGameObject()->GetComponent<AirFollowing>();
	airDash = GetGameObject()->GetComponent<AirDash>();

	airFollow->SetPlayer(target);
	airDash->SetPlayer(target);

	targetDetectionRange = 1000.0f;
	DashTriggerRadius = 300.0f;

	rigidbody = GetGameObject()->GetComponent<Rigidbody>();

	Enemy::Init();
}

void Bomber::OnStart() {

}

void Bomber::OnUpdate(float dt) {
	if (GetGameObject()->Active()) {
		Enemy::OnUpdate(dt);

		if (glm::length(target->GetPosition() - GetGameObject()->m_transform->GetPosition()) < DashTriggerRadius) {
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
	if (GetGameObject()->Active()) {
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