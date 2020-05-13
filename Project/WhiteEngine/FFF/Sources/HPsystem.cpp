#include "HPsystem.hpp"
#include "GameController.hpp"

void HPsystem::SetMaxHP(float hp) {
	this->Maxhp = hp;
	this->hp = Maxhp;
}

void HPsystem::SetHp(float hp) {
	this->hp = hp;
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

void HPsystem::TakeDamage(float damage) {
	if (!this->invincible) 
	{
		if (GameController::GetInstance()->isCursedMode() && (m_gameObject->GetComponent<DeQueen>() == nullptr))
		{
			hp = 0;
			Dead();
		}
		else 
		{
			this->hp -= damage;
			if (m_gameObject->GetComponent<PlayerController>() != nullptr) {
				sp->SetSound(SoundPath("SFX_Player_TakingDamage2"));
				
			}
			else {
				sp->SetSound(SoundPath("SFX_Enemy_TakingDamage"));
			}
		}
		sp->PlaySound();
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
	else {
		sp->SetSound(SoundPath("SFX_Enemy_Killed"));
	}
	sp->PlaySound();
	this->GetGameObject()->SetActive(false);
}

void HPsystem::OnAwake() {
	sp = m_gameObject->GetComponent<SoundPlayer>();
}

void HPsystem::OnEnable() {
	ResetHP();
}

void HPsystem::OnStart() {

}

void HPsystem::OnUpdate(float dt) {

}

void HPsystem::OnFixedUpdate(float dt) {
	if (!dead && hp <= 0)
	{
		hp = 0;
		Dead();
	}
}

void HPsystem::OnDisable() {

}