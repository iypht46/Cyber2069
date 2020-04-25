#include "EnemyBehaviours.h"
#include "Core/Logger.hpp"
#include "Graphic/GLRenderer.h"

Bomber::Bomber() {

}

void Bomber::OnAwake() {
	airFollow = m_gameObject->GetComponent<AirFollowing>();
	airDash = m_gameObject->GetComponent<AirDash>();
	explosion = m_gameObject->GetComponent<Explosion>();

	rigidbody = GetGameObject()->GetComponent<Rigidbody>();

	Enemy::OnAwake();
}

void Bomber::OnUpdate(float dt) {
	if (GetGameObject()->Active()) {
		Enemy::OnUpdate(dt);

		if (glm::length(target->GetPosition() - GetGameObject()->m_transform->GetPosition()) < DashTriggerRadius) {

			GLRenderer::GetInstance()->DrawDebug_Circle(m_gameObject->m_transform->GetPosition().x, m_gameObject->m_transform->GetPosition().y, DashTriggerRadius, 1.0f, 0.0f, 0.0f);
			airDash->TargetLock(target->GetPosition());
			state = EnemyState::Active;
			
		}
		else if (foundTarget && state != EnemyState::Active) {
			state = EnemyState::Chase;
		}
		else if (state != EnemyState::Active) {
			state = EnemyState::Idle;
		}

	}
}

void Bomber::OnFixedUpdate(float dt) {

	if (m_gameObject->Active()) {
		if (!affectedByWeapon) {
			switch (state)
			{
			case EnemyState::Idle:
				rigidbody->SetVelocity(glm::vec3(0));
				airDash->Reset();
				break;
			case EnemyState::Chase:
				airFollow->SetPlayer(target);
				airFollow->FollowPlayer(dt);
				break;
			case EnemyState::Active:
				airDash->Dash(dt);
				if (airDash->DashEnd()) {
					explosion->Explode();
					hpSystem->Dead();

					state = EnemyState::Idle;
				}
				break;
			default:
				break;
			}
		}
	}
}

void Bomber::SetStats(float Speed, float HP, float Dmg, float AimTime, float DashSpeed, float ExplodeDmg, float ExplodeRadius) {
	airFollow->SetFlySpeed(Speed);
	hpSystem->SetMaxHP(HP);

	airDash->SetAimTime(AimTime);
	airDash->SetDashSpeed(DashSpeed);
	airDash->dashDamage = Dmg;

	explosion->SetDamage(ExplodeDmg);
	explosion->SetRadius(ExplodeRadius);
}