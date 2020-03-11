#include "EnemyBehaviours.h"

void Flyer::Init(Transform* player) {
	SetTarget(player);
	//airFollow = m_gameObject->GetComponent<AirFollowing>();
	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();
	groundDash = m_gameObject->GetComponent<GroundDash>();

	//airFollow->SetPlayer(target);
	groundPatrol->Init();
	groundDash->Init();
	targetDetectionRange = 300.0f;
	groundPatrol->SetMaxDelay(0.5f);

	rigidbody = m_gameObject->GetComponent<Rigidbody>();
	rigidbody->SetGravityScale(0);
	Enemy::Init();
}

void Flyer::OnStart() {

}

void Flyer::OnUpdate(float dt) {
	if (m_gameObject->Active()) {
		Enemy::OnUpdate(dt);

		if (glm::length(target->GetPosition() - m_gameObject->m_transform.GetPosition()) < targetDetectionRange && target->GetPosition().y <= m_gameObject->m_transform.GetPosition().y) {
			groundDash->LockTarget(target);
			state = EnemyState::Active;

		}else if (foundTarget && state != EnemyState::Active) {
			state = EnemyState::Chase;
		}
		else {
			state = EnemyState::Idle;
		}
	}
}

void Flyer::OnFixedUpdate(float dt) {
	if (m_gameObject->Active()) {
		switch (state)
		{
		case Idle:
			//rigidbody->SetVelocity(glm::vec3(0));
			//airFollow->FollowPlayer(dt);
			groundPatrol->Patrol(dt);
			break;
		case Chase:
			//airFollow->FollowPlayer(dt);
			groundPatrol->Patrol(dt);
			break;
		case Active:
			groundDash->Dash(dt);
			if (groundDash->DashEnd()) {
				state = EnemyState::Chase;
			}
			break;
		default:
			break;
		}
	}
}