#include "EnemyBehaviours.h"

void Flyer::OnAwake() {
	ENGINE_INFO("flyer init");
	airFollow = GetGameObject()->GetComponent<AirFollowing>();

	airFollow->SetPlayer(target);

	rigidbody = GetGameObject()->GetComponent<Rigidbody>();

	Enemy::OnAwake();
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