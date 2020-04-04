#include "Weapon.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"

#include "Enemy.hpp"

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

void MachineGunBullet::OnFixedUpdate(float dt) {

}

void MachineGunBullet::OnAwake() {
	rb = GetGameObject()->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
}

void MachineGunBullet::OnEnable() {

}

void MachineGunBullet::OnStart() 
{
	rb = GetGameObject()->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
}


void MachineGunBullet::OnDisable() {

}

void MachineGunBullet::OnTriggerEnter(const Physic::Collision col) {
	ENGINE_INFO("Bullet Hit " + col.m_otherCollider->GetGameObject()->Name);

	GetGameObject()->SetActive(false);
	Enemy* enemy = col.m_otherCollider->GetGameObject()->GetComponent<Enemy>();
	if (enemy != nullptr) {
		enemy->TakeDamage(bulletDmg);
	}
}

void MachineGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	GetGameObject()->SetActive(false);
}