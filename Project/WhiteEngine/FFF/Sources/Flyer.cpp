#include "EnemyBehaviours.h"

void Flyer::Init(Transform* player) {
	SetTarget(player);
	airFollow = GetGameObject()->GetComponent<AirFollowing>();

	airFollow->SetPlayer(target);

	targetDetectionRange = 1000.0f;

	rigidbody = GetGameObject()->GetComponent<Rigidbody>();
	Enemy::Init();
}

void Flyer::OnStart() {

}

void Flyer::OnUpdate(float dt) {
	if (GetGameObject()->Active()) {
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
	if (!affectedByWeapon) {
		switch (state)
		{
		case Idle:
			rigidbody->SetVelocity(glm::vec3(0));
			break;
		case Chase:
			airFollow->FollowPlayer(dt);
			break;
		case Active:
		default:
			break;
		}
	}
}

void Flyer::SetStats(float Speed, float HP, float Dmg) 
{
	airFollow->SetFlySpeed(Speed);
	hpSystem->SetMaxHP(HP);
	CollideDamage = Dmg;
}