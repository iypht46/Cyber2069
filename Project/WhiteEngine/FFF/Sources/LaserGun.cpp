#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"
#include "Physic/PhysicScene.hpp"

#include "Graphic/GLRenderer.h"
#include "Graphic/Window.hpp"
#include "Graphic/Camera.hpp"

#include <glm/gtx/rotate_vector.hpp>

LaserGun::LaserGun() 
{

	this->type = WEAPON_TYPE::WEAPON_LASER;

	/*weaponObj = new GameObject();
	weaponObj->AddComponent<MeshRenderer>();
	weaponObj->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
	weaponObj->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/laser_idle.png");
	weaponObj->SetActive(false);*/

	/*laser = new GameObject();
	laser->AddComponent<MeshRenderer>();
	laser->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
	laser->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/laser_bullet2.png");
	laser->SetActive(false);*/
}

void LaserGun::OnAwake() {

	weapon_damage = 10.0f;
	//weapon_firerate = 1.0f;

	laser_size = 25.0f;
	laser_duration = 1.0f;

	m_gameObject->SetActive(false);
	//laser->SetActive(false);
}

void LaserGun::MultiplyWeaponAmplifier(float value) {
	laser_duration = laser_duration * value;
}

void LaserGun::Modify() 
{
}

void LaserGun::GameTimeBehaviour(float dt) 
{
	laserObj->SetActive(false);

	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		Physic::PhysicScene* PhySc = Physic::PhysicScene::GetInstance();

		weapon_delay_count += dt;
		if (weapon_delay_count >= (1.0f / weapon_firerate)) {
			
			laser_duration_count += dt;
			if (laser_duration_count > laser_duration) 
			{
				weapon_delay_count = 0.0f;
				laser_duration_count = 0.0f;
			}

			laserObj->SetActive(true);
			laser_length = (Graphic::Window::GetWidth() / 1.75f) * Graphic::getCamera()->GetZoom();

			if ((*angle > 60 && *angle < 120) || (*angle > 250) || (*angle < -60))
			{
				laser_length = (Graphic::Window::GetHeight() / 1.75f) * Graphic::getCamera()->GetZoom();
			}

			float gun_x = m_gameObject->m_transform->GetPosition().x + (50.0f * glm::cos(glm::radians(*angle)));
			float gun_y = m_gameObject->m_transform->GetPosition().y + (50.0f * glm::sin(glm::radians(*angle)));

			float end_x = m_gameObject->m_transform->GetPosition().x + (laser_length * glm::cos(glm::radians(*angle)));
			float end_y = m_gameObject->m_transform->GetPosition().y + (laser_length * glm::sin(glm::radians(*angle)));

			Physic::RayHit platHit = PhySc->Raycast(Physic::Ray(gun_x, gun_y, end_x, end_y), PhySc->GetLayerFromString("Platform"));

			if (platHit.hit)
			{
				end_x = platHit.position.x;
				end_y = platHit.position.y;
			}

			gunPos = glm::vec2(gun_x, gun_y);
			endPos = glm::vec2(end_x, end_y);

			float laser_length = glm::sqrt(((end_x - gun_x) * (end_x - gun_x)) + ((end_y - gun_y) * (end_y - gun_y)));
			laserObj->m_transform->SetPosition(glm::vec3((end_x + gun_x) / 2, (end_y + gun_y) / 2, 1.0f));
			laserObj->m_transform->SetScale(glm::vec3(laser_length, laser_size, 1.0f));
			laserObj->m_transform->SetRotation(*angle);

			//DamageEnemyInRange();

			//GLRenderer::GetInstance()->DrawDebug_Line(gun_x, gun_y, end_x, end_y, 1.0f, 0.0f, 0.0f);

			//get all collider in target layers
			Physic::RayHits Hits;
			for (std::string target : TargetLayers) {
				Physic::RayHits thislayerHit = PhySc->RaycastAll(Physic::Ray(gun_x, gun_y, end_x, end_y), PhySc->GetLayerFromString(target));
				Hits.insert(Hits.end(), thislayerHit.begin(), thislayerHit.end());
			}

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
}

//void LaserGun::DamageEnemyInRange() {
//	Physic::PhysicScene* ps = Physic::PhysicScene::GetInstance();
//	Physic::Colliders colliders = ps->GetColliderLayer(ps->GetLayerFromString("Enemy"));
//
//	for (Collider* c : colliders) {
//		if (!c->GetGameObject()->Active())
//			continue;
//		
//		if (isInRange(c)) 
//		{
//			Enemy* enemy = c->GetGameObject()->GetComponent<Enemy>();
//			if (enemy != nullptr) {
//				enemy->TakeDamage(weapon_damage);
//			}
//		}
//	}
//
//}
//
//bool LaserGun::isInRange(Collider* col) 
//{
//	glm::vec2 enemPos = glm::vec2(col->GetGameObject()->m_transform->GetPosition().x, col->GetGameObject()->m_transform->GetPosition().y);
//
//	glm::vec2 povitDiff = -1.0f * gunPos;
//
//	Lstart = glm::rotate(gunPos + povitDiff, -1.0f * glm::radians(*angle));
//	Lend = glm::rotate(endPos + povitDiff, -1.0f *  glm::radians(*angle));
//
//	enemPos = glm::rotate(enemPos + povitDiff, -1.0f * glm::radians(*angle));
//
//	BoxCollider* box = dynamic_cast<BoxCollider*>(col->GetGameObject()->GetComponent<Rigidbody>()->GetCollider());
//	glm::vec2 enemSize = glm::vec2(box->GetHw()*2.0f, box->GetHh()*2.0f);
//
//	glm::vec2 laserPos = glm::vec2((Lstart.x + Lend.x)/ 2.0f, 0.0f);
//	glm::vec2 laserSize = glm::vec2((Lstart.x + Lend.x) / 2.0f, laser_size / 2.0f);
//
//	if ((enemPos.x > Lstart.x) && (enemPos.x < Lend.x) && (enemPos.y > (laser_size / - 2.0f) ) && (enemPos.y < (laser_size / 2.0f)))
//	{
//		return true;
//	}
//
//	return false;
//}

void LaserGun::OnDisable() 
{
	if (laserObj != nullptr) {

		laserObj->SetActive(false);
	}
}