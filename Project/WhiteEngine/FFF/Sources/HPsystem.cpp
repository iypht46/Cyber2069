#include "HPsystem.hpp"
#include "GameController.hpp"
#include "Scripts/GameControl/CameraController.h"

void HPsystem::SetMaxHP(float hp) {
	this->Maxhp = hp;
	this->hp = Maxhp;
}

void HPsystem::SetHp(float hp) {
	this->hp = glm::clamp(hp, 0.0f, Maxhp);
}

void HPsystem::SetInvincible(bool inv) 
{
	this->invincible = inv;
}

float HPsystem::GetMaxHP() {
	return this->Maxhp;
}

float HPsystem::GetHP() {
	return this->hp;
}

void HPsystem::ResetHP() {
	this->hp = Maxhp;
	this->dead = false;
}

void HPsystem::OnEnable() 
{
	ResetHP();
}

void HPsystem::TakeDamage(float damage) {
	if (!this->invincible && !dead) {

		//shake camera 
		if (m_gameObject->GetComponent<PlayerController>() != nullptr) {
			CameraController::GetInstance()->ShakeCamera(30.0f, 10.0f, 0.2f);
			GameController::GetInstance()->ResetCombo();
			sp->SetSound(SoundPath("SFX_Player_TakingDamage2"));
			

		}
		else {
			sp->SetSound(SoundPath("SFX_Enemy_TakingDamage"));
		}

		sp->PlaySound();

		if (GameController::GetInstance()->isCursedMode() && (m_gameObject->GetComponent<DeQueen>() == nullptr))
		{
			hp = 0;
			Dead();
		}
		else {
			this->hp -= damage;

			if (!dead && hp <= 0)
			{
				hp = 0;
				Dead();
			}
		}
	}
}

bool HPsystem::isDead() {
	return this->dead;
}

bool HPsystem::isInvicible() {
	return this->invincible;
}

void HPsystem::Dead() 
{
	this->dead = true;
	if (m_gameObject->GetComponent<PlayerController>() != nullptr) {
		sp->SetSound(SoundPath("SFX_Player_Killed"));

	}
	sp->PlaySound();
}

void HPsystem::TurnOff() {
	this->GetGameObject()->SetActive(false);
}

void HPsystem::OnAwake() {
	sp = m_gameObject->GetComponent<SoundPlayer>();
}

void HPsystem::OnStart() {

}

void HPsystem::OnUpdate(float dt) {

}

void HPsystem::OnFixedUpdate(float dt) {
	if (!dead && hp <= 0)
	{
		hp = 0;

		this->dead = true;
	}

	if (dead) 
	{
		deadDelayCount += dt;

		if (deadDelayCount >= deadDelay) 
		{
			deadDelayCount = 0;
			TurnOff();
		}
	}
}

void HPsystem::OnDisable() {

}