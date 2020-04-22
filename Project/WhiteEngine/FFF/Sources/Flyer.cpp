#include "EnemyBehaviours.h"

void Flyer::OnAwake() {
	ENGINE_INFO("flyer init");
	airFollow = GetGameObject()->GetComponent<AirFollowing>();

	rigidbody = GetGameObject()->GetComponent<Rigidbody>();

	FlyerSound->CreateSoundPlayer();
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
			airFollow->SetPlayer(target);
			airFollow->FollowPlayer(dt);
			FlyerSound->SetSound("Assets/EnemySFX/SFX_FlyingEnemy_Fly.wav");
			FlyerSound->SetLoop(true);
			FlyerSound->PlaySound();
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

	//will add air dash
	CollideDamage = Dmg;
}