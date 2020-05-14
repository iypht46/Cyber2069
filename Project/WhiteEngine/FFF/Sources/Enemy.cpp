#include "Enemy.hpp"
#include "GameController.hpp"

void Enemy::OnAwake() {
	hpSystem = GetGameObject()->GetComponent<HPsystem>();
	animator = GetGameObject()->GetComponent<Animator>();
}

void Enemy::OnTakeDamage() {
	//call all funct from events vector
}

void Enemy::OnDead() {
	//call all funct from events vector
	//GetGameObject()->SetActive(false);
	GameController::GetInstance()->AddScoreValue(baseScore);
	GameController::GetInstance()->AddComboValue(1.0);

	if (m_gameObject->GetComponent<Bomber>() == nullptr) {

		m_gameObject->GetComponent<Rigidbody>()->SetGravityScale(1.0f);
	}
	else
	{
		if (!m_gameObject->GetComponent<Bomber>()->explode) {

			m_gameObject->GetComponent<Rigidbody>()->SetGravityScale(1.0f);
		}
	}

	m_gameObject->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(0.0f));

	DeQueen* queen = m_gameObject->GetComponent<DeQueen>();

	if (queen != nullptr) {
		queen->SpawnItem();
	}
}

void Enemy::OnEnable() 
{
	state = EnemyState::Idle;
	GotZap = false;
	affectedByWeapon = false;
	isDead = false;
	foundTarget = false;

	setAnimDash = false;
	setAnimDead = false;

	if ((m_gameObject->GetComponent<AirFollowing>() != nullptr) || (m_gameObject->GetComponent<AirPatrol>() != nullptr) || (m_gameObject->GetComponent<Cocoon>() != nullptr))
	{
		m_gameObject->GetComponent<Rigidbody>()->SetGravityScale(0.00000001f);
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
	if (!hpSystem->isDead()) {
		hpSystem->TakeDamage(damage);
		OnTakeDamage();

		//if dead
		if (hpSystem->isDead())
		{
			if (!isDead)
			{
				isDead = true;
				affectedByWeapon = true;

				OnDead();
			}
		}
	}
}