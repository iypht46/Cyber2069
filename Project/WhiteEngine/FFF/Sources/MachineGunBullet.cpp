#include "Weapon.hpp"

#include "Enemy.hpp"
#include "GameController.hpp"

#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"

#include "Core/Particle/ParticleSystem.h"

void MachineGunBullet::OnUpdate(float dt)
{
	int winWidth;
	int winHeight;

	glm::vec3 camPos = cam->GetCampos();

	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	if ((GetGameObject()->m_transform->GetPosition().x > (camPos.x + (winWidth/2)))
		|| (GetGameObject()->m_transform->GetPosition().x < (camPos.x - (winWidth / 2)))
		|| (GetGameObject()->m_transform->GetPosition().y > (camPos.y + (winHeight / 2)))
		|| (GetGameObject()->m_transform->GetPosition().y < (camPos.y - (winHeight / 2))))
	{
		rb->SetVelocity(glm::vec3(0));
		GetGameObject()->SetActive(false);
	}
}

void MachineGunBullet::OnAwake() {
	rb = GetGameObject()->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
}

void MachineGunBullet::OnTriggerEnter(const Physic::Collision col) {
	//ENGINE_INFO("Bullet Hit " + col.m_otherCollider->GetGameObject()->Name);

	if (isTarget(col.m_otherCollider->GetGameObject()->Layer)) {
		Enemy* enemy = col.m_otherCollider->GetGameObject()->GetComponent<Enemy>();
		if (enemy != nullptr) {
			enemy->TakeDamage(bulletDmg);
		}
	}

	//particle
	GameObject* hit = GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_BULLET_HIT_ENEMY)->GetGameObject();
	hit->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
	hit->GetComponent<ParticleSystem>()->TriggerBurstEmission();

	GetGameObject()->SetActive(false);
}

void MachineGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	//particle
	GameObject* hit = GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_BULLET_HIT_ARMOR)->GetGameObject();
	hit->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
	hit->GetComponent<ParticleSystem>()->TriggerBurstEmission();

	GetGameObject()->SetActive(false);
}
