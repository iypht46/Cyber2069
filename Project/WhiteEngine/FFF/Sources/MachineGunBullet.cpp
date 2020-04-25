#include "Weapon.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"

#include "Enemy.hpp"
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

	Enemy* enemy = col.m_otherCollider->GetGameObject()->GetComponent<Enemy>();
	if (enemy != nullptr) {
		enemy->TakeDamage(bulletDmg);
	}

	GetGameObject()->SetActive(false);

	//particle
	m_gameObject->GetComponent<ParticleSystem>()->TriggerBurstEmission();
}

void MachineGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	GetGameObject()->SetActive(false);

	//particle
	m_gameObject->GetComponent<ParticleSystem>()->TriggerBurstEmission();
}