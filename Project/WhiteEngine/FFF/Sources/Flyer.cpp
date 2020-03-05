#include "EnemyBehaviours.h"

void Flyer::Init(Transform* player) {
	SetTarget(player);
	//airFollow = m_gameObject->GetComponent<AirFollowing>();
	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();

	//airFollow->SetPlayer(target);
	groundPatrol->Init();
	targetDetectionRange = 1000.0f;

	rigidbody = m_gameObject->GetComponent<Rigidbody>();
	rigidbody->SetGravityScale(0);
	Enemy::Init();
}

void Flyer::OnStart() {

}

void Flyer::OnUpdate(float dt) {
	if (m_gameObject->Active()) {
		Enemy::OnUpdate(dt);

		if (foundTarget) {
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
		default:
			break;
		}
	}
}