#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"
#include "Physic/PhysicScene.hpp"

#include "Graphic/GLRenderer.h"
#include "Graphic/Window.hpp"
#include "Graphic/Camera.hpp"

LaserGun::LaserGun() 
{
	weaponObj = new GameObject();
	weaponObj->AddComponent<MeshRenderer>();
	weaponObj->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
	weaponObj->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/laser_idle.png");
	weaponObj->SetActive(false);

	laser = new GameObject();
	laser->AddComponent<MeshRenderer>();
	laser->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
	laser->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/laser_bullet2.png");
	laser->SetActive(false);

	weaponObj->AddComponent<SoundPlayer>();
	weaponObj->GetComponent<SoundPlayer>()->CreateSoundPlayer();
	weaponObj->GetComponent<SoundPlayer>()->SetSound(SoundPath("SFX_Laser_Shoot"));
	weaponObj->GetComponent<SoundPlayer>()->SetLoop(false);

	weapon_damage = 10.0f;

	weapon_scale.x = 70.0f;
	weapon_scale.y = 70.0f;
}

void LaserGun::Modify(GameObject* obj) 
{

}

void LaserGun::GameTimeBehaviour(float dt) 
{
	laser->SetActive(false);

	if (Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT)) {
		weaponObj->GetComponent<SoundPlayer>()->PlaySound();
	}

	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		Physic::PhysicScene* PhySc = Physic::PhysicScene::GetInstance();

		laser->SetActive(true);
		laser_length = (Graphic::Window::GetWidth() / 1.75f) * Graphic::getCamera()->GetZoom();

		if ((*angle > 60 && *angle < 120) || (*angle > 250) || (*angle < -60))
		{
			laser_length = (Graphic::Window::GetHeight() / 1.75f) * Graphic::getCamera()->GetZoom();
		}

		float gun_x = weaponObj->m_transform->GetPosition().x + (50.0f * glm::cos(glm::radians(*angle)));
		float gun_y = weaponObj->m_transform->GetPosition().y + (50.0f * glm::sin(glm::radians(*angle)));

		float end_x = weaponObj->m_transform->GetPosition().x + (laser_length * glm::cos(glm::radians(*angle)));
		float end_y = weaponObj->m_transform->GetPosition().y + (laser_length * glm::sin(glm::radians(*angle)));

		Physic::RayHit platHit = PhySc->Raycast(Physic::Ray(gun_x, gun_y, end_x, end_y), PhySc->GetLayerFromString("Platform"));

		if (platHit.hit) 
		{
			end_x = platHit.position.x;
			end_y = platHit.position.y;
		}

		float laser_length = glm::sqrt(((end_x - gun_x) * (end_x - gun_x)) + ((end_y - gun_y) * (end_y - gun_y)));
		laser->m_transform->SetPosition(glm::vec3((end_x + gun_x) / 2, (end_y + gun_y) / 2, 1.0f));
		laser->m_transform->SetScale(glm::vec3(laser_length, 25.0f, 1.0f));
		laser->m_transform->SetRotation(*angle);

		//GLRenderer::GetInstance()->DrawDebug_Line(gun_x, gun_y, end_x, end_y, 1.0f, 0.0f, 0.0f);
		Physic::RayHits Hits = PhySc->RaycastAll(Physic::Ray(gun_x, gun_y, end_x, end_y), PhySc->GetLayerFromString("Enemy"));

		for (Physic::RayHit h : Hits) 
		{
			Enemy* enemy = h.collider->GetGameObject()->GetComponent<Enemy>();

			if (enemy != nullptr)
			{
				enemy->TakeDamage(weapon_damage);
			}
		}
	}
}

void LaserGun::onDisable() 
{
	weaponObj->SetActive(false);
	laser->SetActive(false);
}