#include "Weapon.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"
#include "Physic/PhysicScene.hpp"
#include "Graphic/GLRenderer.h"

#include "Enemy.hpp"

void ZapperGunBullet::OnUpdate(float dt)
{
	int winWidth;
	int winHeight;

	glm::vec3 camPos = cam->GetCampos();

	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	if ((m_gameObject->m_transform.GetPosition().x > (camPos.x + (winWidth / 2)))
		|| (m_gameObject->m_transform.GetPosition().x < (camPos.x - (winWidth / 2)))
		|| (m_gameObject->m_transform.GetPosition().y > (camPos.y + (winHeight / 2)))
		|| (m_gameObject->m_transform.GetPosition().y < (camPos.y - (winHeight / 2))))
	{
		rb->SetVelocity(glm::vec3(0));
		m_gameObject->SetActive(false);
	}
}

void ZapperGunBullet::OnFixedUpdate(float dt) {

}

void ZapperGunBullet::OnAwake() {

}

void ZapperGunBullet::OnEnable() {

}

void ZapperGunBullet::OnStart()
{
	rb = m_gameObject->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
}


void ZapperGunBullet::OnDisable() {

}

void ZapperGunBullet::OnTriggerEnter(const Physic::Collision col) {
	ENGINE_INFO("Bullet Hit " + col.m_otherCollider->GetGameObject()->Name);

	m_gameObject->SetActive(false);
	
	Enemy* enemy = col.m_otherCollider->GetGameObject()->GetComponent<Enemy>();
	if (enemy != nullptr) {
		target = enemy;

		for (int i = 0; i < chainNumber; i++) 
		{
			Zap(target);

			if (target == nullptr) 
			{
				break;
			}
		}
	}
}

void ZapperGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	m_gameObject->SetActive(false);
}

void ZapperGunBullet::Zap(Enemy* e)
{
	e->TakeDamage(bulletDmg);

	Physic::PhysicScene* ps = Physic::PhysicScene::GetInstance();
	Physic::Colliders colliders = ps->GetColliderLayer(ps->GetLayerFromString("Enemy"));

	float minDistance = -1;
	Enemy* nextTarget = nullptr;

	for (Collider* c : colliders) {
		if ((!c->GetGameObject()->Active()))
		{
			continue;
		}

		float distance = glm::length(c->GetGameObject()->m_transform.GetPosition() - e->GetGameObject()->m_transform.GetPosition());

		if (distance > zapDistance)
		{
			continue;
		}
		
		if (minDistance == -1) 
		{
			minDistance = distance;
			nextTarget = c->GetGameObject()->GetComponent<Enemy>();
		}
		else if(distance < minDistance) 
		{
			minDistance = distance;
			nextTarget = c->GetGameObject()->GetComponent<Enemy>();
		}
	}

	if (nextTarget == nullptr) {
		target = nullptr;
	}
	else {
		GAME_INFO("Set");
		target = nextTarget;

		GLRenderer::GetInstance()->DrawDebug_Line(e->GetGameObject()->m_transform.GetPosition().x, e->GetGameObject()->m_transform.GetPosition().y
			, target->GetGameObject()->m_transform.GetPosition().x, target->GetGameObject()->m_transform.GetPosition().y
			, 0.0f, 0.0f, 1.0f);
	}
}