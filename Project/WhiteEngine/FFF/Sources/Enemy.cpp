#include "Enemy.hpp"
#include "GameController.hpp"

void Enemy::Init() {
	hpSystem = m_gameObject->GetComponent<HPsystem>();
	animator = m_gameObject->GetComponent<Animator>();
}

void Enemy::OnTakeDamage() {
	//call all funct from events vector
}

void Enemy::OnDead() {
	//call all funct from events vector
	m_gameObject->SetActive(false);
	GameController::GetInstance()->AddScoreValue(1.0);
}

//check if target in range
void Enemy::OnUpdate(float dt) {
	if (target != nullptr) {
		foundTarget = glm::length(target->GetPosition() - m_gameObject->m_transform->GetPosition()) < targetDetectionRange;
	}
}

void Enemy::SetTarget(std::shared_ptr<Transform> target) {
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