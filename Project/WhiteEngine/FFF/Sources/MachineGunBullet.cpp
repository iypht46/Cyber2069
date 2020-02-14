#include "MachineGunBullet.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"

#include "FlyerBehaviour.hpp"

void MachineGunBullet::OnUpdate(float dt)
{
	glm::vec3 camPos = cam->GetCampos();


	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	if ((m_gameObject->m_transform.GetPosition().x > (camPos.x + (winWidth/2)))
		|| (m_gameObject->m_transform.GetPosition().x < (camPos.x - (winWidth / 2)))
		|| (m_gameObject->m_transform.GetPosition().y > (camPos.y + (winHeight / 2)))
		|| (m_gameObject->m_transform.GetPosition().y < (camPos.y - (winHeight / 2))))
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
	//ENGINE_INFO("Bullet Hit");

	m_gameObject->SetActive(false);
	FlyerBehaviour* other = col.m_otherCollider->GetGameObject()->GetComponent<FlyerBehaviour>();
	if (--(other->hp) <= 0) {
		col.m_otherCollider->GetGameObject()->SetActive(false);
	}
}

void MachineGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	m_gameObject->SetActive(false);
}