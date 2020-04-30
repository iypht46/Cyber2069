#include "Weapon.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"
#include "Physic/PhysicScene.hpp"
#include "Graphic/GLRenderer.h"

#include "Enemy.hpp"

void BlackholeGunBullet::OnUpdate(float dt)
{
	int winWidth;
	int winHeight;

	glm::vec3 camPos = cam->GetCampos();

	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	if ((m_gameObject->m_transform->GetPosition().x > (camPos.x + (winWidth / 2)))
		|| (m_gameObject->m_transform->GetPosition().x < (camPos.x - (winWidth / 2)))
		|| (m_gameObject->m_transform->GetPosition().y > (camPos.y + (winHeight / 2)))
		|| (m_gameObject->m_transform->GetPosition().y < (camPos.y - (winHeight / 2))))
	{
		if (!isTriggerEnemy) 
		{
			rb->SetVelocity(glm::vec3(0));
			m_gameObject->SetActive(false);
		}
	}

	if (isTriggerEnemy) {
		rb->SetVelocity(glm::vec3(0));
		DurationCount += dt;

		DragEnemy(dt);

		GLRenderer::GetInstance()->DrawDebug_Circle(m_gameObject->m_transform->GetPosition().x, m_gameObject->m_transform->GetPosition().y, Radius, 0.0f, 0.0f, 1.0f);

		if (DurationCount >= Duration) 
		{
			ReleaseEnemy();
			isTriggerEnemy = false;
			m_gameObject->SetActive(false);
		}
	}

}

void BlackholeGunBullet::OnAwake()
{
	rb = m_gameObject->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
}

void BlackholeGunBullet::OnTriggerEnter(const Physic::Collision col) {
	ENGINE_INFO("Bullet Hit " + col.m_otherCollider->GetGameObject()->Name);

	Enemy* enemy = col.m_otherCollider->GetGameObject()->GetComponent<Enemy>();
	if (enemy != nullptr && (enemy->GetGameObject()->Active())) {
		if (!isTriggerEnemy) {
			isTriggerEnemy = true;
			DurationCount = 0.0f;
		}
	}
}

void BlackholeGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	m_gameObject->SetActive(false);
}

void BlackholeGunBullet::DragEnemy(float dt) 
{
	Rigidbody* enemRb;

	Physic::PhysicScene* ps = Physic::PhysicScene::GetInstance();
	Physic::Colliders colliders = ps->GetColliderLayer(ps->GetLayerFromString("Enemy"));
	Physic::Colliders collide;


	Dot_count += dt;
	for (Collider* c : colliders) {
		float distance = glm::length(c->GetGameObject()->m_transform->GetPosition() - m_gameObject->m_transform->GetPosition());
		
		if (distance > Radius) {
			continue;
		}
		else {
			Enemy* enemy = c->GetGameObject()->GetComponent<Enemy>();
			if (enemy != nullptr && (enemy->GetGameObject()->Active())) {

				enemRb = enemy->GetGameObject()->GetComponent<Rigidbody>();

				enemy->SetAffectedByWeapon(true);
				float angle = atan2((m_gameObject->m_transform->GetPosition().y - c->GetGameObject()->m_transform->GetPosition().y), (m_gameObject->m_transform->GetPosition().x - c->GetGameObject()->m_transform->GetPosition().x));

				enemRb->SetVelocity(glm::vec3(ToCenterSpeed * cos(angle), ToCenterSpeed * sin(angle), 0.0f));


				if (distance < 10) 
				{
					enemRb->SetVelocity(glm::vec3(0.0f));

					if (Dot_count >= 1.0f) 
					{
						enemy->TakeDamage(bulletDmg);
					}
				}
			}
		}
	}

	if (Dot_count >= 1.0f)
	{
		Dot_count = 0.0f;
	}
}

void BlackholeGunBullet::ReleaseEnemy() {
	Physic::PhysicScene* ps = Physic::PhysicScene::GetInstance();
	Physic::Colliders colliders = ps->GetColliderLayer(ps->GetLayerFromString("Enemy"));
	Physic::Colliders collide;

	for (Collider* c : colliders) {
		float distance = glm::length(c->GetGameObject()->m_transform->GetPosition() - m_gameObject->m_transform->GetPosition());

		if (distance > Radius) {
			continue;
		}
		else {
			Enemy* enemy = c->GetGameObject()->GetComponent<Enemy>();
			if (enemy != nullptr && (enemy->GetGameObject()->Active())) {

				enemy->SetAffectedByWeapon(false);
				enemy->SetState(EnemyState::Idle);
			}
		}
	}
}