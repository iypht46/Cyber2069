#include "EnemyBehaviours.h"

void Charger::OnAwake() {
	groundPatrol = m_gameObject->GetComponent<GroundPatrol>();
	groundDash = m_gameObject->GetComponent<GroundDash>();

	rigidbody = m_gameObject->GetComponent<Rigidbody>();

	Enemy::OnAwake();
}

void Charger::OnEnable() {
	animator->setCurrentState(0);
}

void Charger::SetStats(float Speed, float HP, float DashPauseTime, float DashSpeed, float Dmg) {
	groundPatrol->SetSpeed(Speed);
	hpSystem->SetMaxHP(HP);
	groundDash->SetPauseTime(DashPauseTime);
	groundDash->SetDashSpeed(DashSpeed);
	groundDash->dashDamage = Dmg;
}

void Charger::OnUpdate(float dt) {
	Enemy::OnUpdate(dt);

	//if target in dash range
	if (state != EnemyState::Dash && foundTarget && glm::abs(target->GetPosition().y - m_gameObject->m_transform->GetPosition().y) <= DashTriggerRangeY) {
		groundDash->LockTarget(target);
		state = EnemyState::Dash;
		animator->setCurrentState(1);
	}
	else if(!foundTarget || glm::abs(target->GetPosition().y - m_gameObject->m_transform->GetPosition().y) >= DashTriggerRangeY){
		state = EnemyState::Idle;
		animator->setNextState(0);
	}
}

void Charger::OnFixedUpdate(float dt) {
	if (!affectedByWeapon) {

		switch (state)
		{
		case Idle:
			groundPatrol->Patrol(dt);
			break;

		case Dash:
		{
			groundDash->Dash(dt);

			if (!dashingTmp && groundDash->Dashing()) {
				animator->setCurrentState(2);
			}

			if (groundDash->DashEnd()) {
				state = EnemyState::Idle;
			}

			dashingTmp = groundDash->Dashing();
		}
		break;

		default:
			break;
		}
	}

	if (isDead && !setAnimDead) {
		setAnimDead = true;
		animator->setCurrentState(3);
	}
	
}