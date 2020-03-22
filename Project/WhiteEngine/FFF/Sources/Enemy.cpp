#include "Enemy.hpp"
#include "GameController.hpp"

void Enemy::Init() {
	hpSystem = GetGameObject()->GetComponent<HPsystem>();
	animator = GetGameObject()->GetComponent<Animator>();
}

void Enemy::OnTakeDamage() {
	//call all funct from events vector
}

void Enemy::OnDead() {
	//call all funct from events vector
	GetGameObject()->SetActive(false);
	GameController::GetInstance()->AddScoreValue(1.0);
}

//check if target in range
void Enemy::OnUpdate(float dt) {
	if (target != nullptr) {
		foundTarget = glm::length(target->GetPosition() - GetGameObject()->m_transform->GetPosition()) < targetDetectionRange;
	}
}

void Enemy::SetTarget(Transform* target) {
	this->target = target;
}

void Enemy::TakeDamage(float damage) {
	hpSystem->TakeDamage(damage);
	OnTakeDamage();

	//if dead
	if (hpSystem->GetHP() <= 0) {
		OnDead();
	}
}