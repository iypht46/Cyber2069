#include "Enemy.hpp"

void Enemy::OnTakeDamage() {
	//call all funct from events vector
}

void Enemy::OnDead() {
	//call all funct from events vector
}

//check if target in range
void Enemy::OnUpdate(float dt) {
	foundTarget = glm::length(target->GetPosition() - m_gameObject->m_transform.GetPosition()) < targetDetectionRange;
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