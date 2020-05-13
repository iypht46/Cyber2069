#include "Weapon.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"
#include "Physic/PhysicScene.hpp"
#include "Graphic/GLRenderer.h"

#include "Enemy.hpp"
#include "PlayerController.hpp"

void GrenadeLauncherBullet::OnUpdate(float dt)
{
	int winWidth;
	int winHeight;

	glm::vec3 camPos = cam->GetCampos();

	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	float velo_x = rb->GetVelocity().x;
	float velo_y = rb->GetVelocity().y;

	float bulletAngle = glm::degrees(glm::atan(velo_y / velo_x));

	m_gameObject->m_transform->SetRotation(bulletAngle);

	if (velo_x < 0) {
		m_gameObject->m_transform->SetScale(glm::vec3(scaleX * -1.0f, m_gameObject->m_transform->GetScale().y, m_gameObject->m_transform->GetScale().z));
	}
	else {
		m_gameObject->m_transform->SetScale(glm::vec3(scaleX * 1.0f, m_gameObject->m_transform->GetScale().y, m_gameObject->m_transform->GetScale().z));
	}
	
	if ((m_gameObject->m_transform->GetPosition().x > (camPos.x + (winWidth / 2)))
		|| (m_gameObject->m_transform->GetPosition().x < (camPos.x - (winWidth / 2)))
		|| (m_gameObject->m_transform->GetPosition().y > (camPos.y + (winHeight / 2)))
		|| (m_gameObject->m_transform->GetPosition().y < (camPos.y - (winHeight / 2))))
	{
		rb->SetVelocity(glm::vec3(0));
		m_gameObject->SetActive(false);
	}
}

void GrenadeLauncherBullet::OnAwake()
{
	rb = m_gameObject->GetComponent<Rigidbody>();
	ExplodeSound = m_gameObject->GetComponent<SoundPlayer>();
	ExplodeSound->SetSound(SoundPath("SFX_Grenade_Explode"));
	cam = Graphic::getCamera();
	scaleX = m_gameObject->m_transform->GetScale().x;
}

void GrenadeLauncherBullet::OnTriggerEnter(const Physic::Collision col) {
	//ENGINE_INFO("Bullet Hit " + col.m_otherCollider->GetGameObject()->Name);

	m_gameObject->SetActive(false);
	
	Explode();
}

void GrenadeLauncherBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	m_gameObject->SetActive(false);

	Explode();
}

void GrenadeLauncherBullet::Explode() {
	Physic::PhysicScene* ps = Physic::PhysicScene::GetInstance();

	//get all collider in target layers
	Physic::Colliders targetColliders;
	for (std::string target : TargetLayers) {
		Physic::Colliders layerColliders = ps->GetColliderLayer(target);
		targetColliders.insert(targetColliders.end(), layerColliders.begin(), layerColliders.end());
	}

	for (Collider* c : targetColliders) {
		if (c->GetGameObject()->Active()) {
			float distance = glm::length(c->GetGameObject()->m_transform->GetPosition() - m_gameObject->m_transform->GetPosition());

			if (distance <= radius) {

				//check for enemy anyway
				Enemy* enemy = c->GetGameObject()->GetComponent<Enemy>();
				if (enemy != nullptr) {
					enemy->TakeDamage(bulletDmg);
				}
			}
		}
	}

	ExplodeSound->PlaySound();

	GLRenderer::GetInstance()->DrawDebug_Circle(m_gameObject->m_transform->GetPosition().x, m_gameObject->m_transform->GetPosition().y, radius, 0.0f, 0.0f, 1.0f);
}