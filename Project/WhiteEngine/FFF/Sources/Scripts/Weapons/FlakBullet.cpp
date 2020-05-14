#include "../../Weapon.hpp"
#include "../../Explosion.hpp"
#include "Core/Particle/ParticleSystem.h"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"
#include "Physic/PhysicScene.hpp"
#include "Graphic/GLRenderer.h"

void FlakBullet::OnUpdate(float dt)
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
		rb->SetVelocity(glm::vec3(0));
		m_gameObject->SetActive(false);
	}

	//if reach destination
	if (glm::length((glm::vec2)m_gameObject->m_transform->GetPosition() - Destination) <= glm::length(rb->GetVelocity()) * dt * 2) {
		Explode();
	}
}

void FlakBullet::OnAwake()
{
	rb = m_gameObject->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
	explosion = m_gameObject->GetComponent<Explosion>();
	explosion->TargetLayers = TargetLayers;
	particle = m_gameObject->GetComponent<ParticleSystem>();
	sp = m_gameObject->GetComponent<SoundPlayer>();
	sp->SetSound(SoundPath("SFX_SpitterBullet_Explode"));
}

void FlakBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	m_gameObject->SetActive(false);
}

void FlakBullet::Explode() {
	rb->SetVelocity(glm::vec3(0));

	//particle
	particle->TriggerBurstEmission();

	//sound
	sp->PlaySound();

	//deal damage
	explosion->Explode();

	m_gameObject->SetActive(false);
}
