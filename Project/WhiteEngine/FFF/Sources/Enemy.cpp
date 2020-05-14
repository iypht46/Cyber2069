#include "Enemy.hpp"
#include "GameController.hpp"

void Enemy::OnAwake() {
	hpSystem = GetGameObject()->GetComponent<HPsystem>();
	animator = GetGameObject()->GetComponent<Animator>();
	sp = GetGameObject()->GetComponent<SoundPlayer>();
}

void Enemy::OnTakeDamage() {
	//call all funct from events vector
}

void Enemy::OnDead() {
	//call all funct from events vector
	GetGameObject()->SetActive(false);
	GameController::GetInstance()->AddScoreValue(baseScore);
	GameController::GetInstance()->AddComboValue(1.0);
	state = EnemyState::Idle;
	affectedByWeapon = false;
	GotZap = false;

	if (m_gameObject->GetComponent<Cocoon>() != nullptr) {
	sp->SetSound(SoundPath("SFX_Game_Cocoon_Killed"));
	}
	else {
	sp->SetSound(SoundPath("SFX_Enemy_Killed"));
	}

	sp->PlaySound();

	DeQueen* queen = m_gameObject->GetComponent<DeQueen>();

	if (queen != nullptr) {
		queen->SpawnItem();
	}
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