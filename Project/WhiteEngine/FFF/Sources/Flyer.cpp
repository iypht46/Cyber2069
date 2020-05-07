#include "EnemyBehaviours.h"

void Flyer::OnAwake() {
	ENGINE_INFO("flyer init");
	airFollow = GetGameObject()->GetComponent<AirFollowing>();

	rigidbody = GetGameObject()->GetComponent<Rigidbody>();

	FlyerSound = GetGameObject()->GetComponent<SoundPlayer>();
	SoundCounter = 20;
	FlyerSound->CreateSoundPlayer();
	FlyerSound->SetSound(SoundPath("SFX_FlyingEnemy_Fly"));
	FlyerSound->SetLoop(false);
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
			SoundCounter -= dt;
			if (SoundCounter <= 0) {
				FlyerSound->PlaySound();
				SoundCounter = 20;
			}
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