#include "MachineGunBullet.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"

#include "Enemy.hpp"

void MachineGunBullet::OnUpdate(float dt)
{
	glm::vec3 camPos = cam->GetCampos();


	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	if ((m_gameObject->m_transform->GetPosition().x > (camPos.x + (winWidth/2)))
		|| (m_gameObject->m_transform->GetPosition().x < (camPos.x - (winWidth / 2)))
		|| (m_gameObject->m_transform->GetPosition().y > (camPos.y + (winHeight / 2)))
		|| (m_gameObject->m_transform->GetPosition().y < (camPos.y - (winHeight / 2))))
	{
		rb->SetVelocity(glm::vec3(0));
		m_gameObject->SetActive(false);
	}
}

void MachineGunBullet::OnFixedUpdate(float dt) {

}

void MachineGunBullet::OnAwake() {

}

void MachineGunBullet::OnEnable() {

}

void MachineGunBullet::OnStart() 
{
	rb = m_gameObject->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
}


void MachineGunBullet::OnDisable() {

}

void MachineGunBullet::OnTriggerEnter(const Physic::Collision col) {
	ENGINE_INFO("Bullet Hit " + col.m_otherCollider->GetGameObject()->Name);

	m_gameObject->SetActive(false);
	std::shared_ptr<Enemy> enemy = col.m_otherCollider->GetGameObject()->GetComponent<Enemy>();
	if (enemy != nullptr) {
		enemy->TakeDamage(1.0f);
	}
}

void MachineGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	m_gameObject->SetActive(false);
}