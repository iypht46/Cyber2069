#include "HPsystem.hpp"

void HPsystem::SetMaxHP(float hp) {
	this->Maxhp = hp;
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
		this->hp -= damage;
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
	this->GetGameObject()->SetActive(false);
}

void HPsystem::OnAwake() {

}

void HPsystem::OnEnable() {
	ResetHP();
}

void HPsystem::OnStart() {

}

void HPsystem::OnUpdate(float dt) {
	if (!dead && hp <= 0)
	{
		hp = 0;
		Dead();
	}
}

void HPsystem::OnFixedUpdate(float dt) {

}

void HPsystem::OnDisable() {

}