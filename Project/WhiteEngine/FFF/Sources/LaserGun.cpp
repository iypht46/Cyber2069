#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"
#include "Physic/PhysicScene.hpp"

#include "Graphic/GLRenderer.h"
#include "Graphic/Window.hpp"
#include "Graphic/Camera.hpp"

#include <glm/gtx/rotate_vector.hpp>

#include "Core/Particle/ParticleSystem.h"

LaserGun::LaserGun() 
{

	this->type = WEAPON_TYPE::WEAPON_LASER;

	TargetLayers.insert("Enemy");
	TargetLayers.insert("GroundEnemy");

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

	m_gameObject->GetComponent<SoundPlayer>()->CreateSoundPlayer();
	m_gameObject->GetComponent<SoundPlayer>()->SetSound(SoundPath("SFX_Laser_Shoot"));
	m_gameObject->GetComponent<SoundPlayer>()->SetLoop(false);

	weapon_damage = 10.0f;
	//weapon_firerate = 1.0f;

	laser_size = 10.0f;
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

	if (Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT)) {
		m_gameObject->GetComponent<SoundPlayer>()->PlaySound();
	}

	weapon_delay_count += dt;

	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		if (!initparticle) {
			getParticles();
		}

		Physic::PhysicScene* PhySc = Physic::PhysicScene::GetInstance();


		if (weapon_delay_count >= (1.0f / weapon_firerate)) {

			
			if (laser_duration_count == 0.0f)
			{
				m_gameObject->GetComponent<SoundPlayer>()->PlaySound();
			}

			laser_duration_count += dt;
			if (laser_duration_count > laser_duration) 
			{
				weapon_delay_count = 0.0f;
				laser_duration_count = 0.0f;
			}

			laserObj->SetActive(true);
			laser_length = (Graphic::Window::GetWidth() / 1.5f) * Graphic::getCamera()->GetZoom();

			if ((*angle > 60 && *angle < 120) || (*angle > 250) || (*angle < -60))
			{
				laser_length = (Graphic::Window::GetHeight() / 1.5f) * Graphic::getCamera()->GetZoom();
			}

			/*float gun_x = m_gameObject->m_transform->GetPosition().x + (50.0f * glm::cos(glm::radians(*angle)));
			float gun_y = m_gameObject->m_transform->GetPosition().y + (50.0f * glm::sin(glm::radians(*angle)));*/

			float gun_x = m_gameObject->m_transform->GetPosition().x;
			float gun_y = m_gameObject->m_transform->GetPosition().y;
				
			Transform* player = modifyObject->m_transform.get();

			if (player->GetScale().x < 0) {

				float dist = glm::abs(m_gameObject->m_transform->GetPosition().x - player->GetPosition().x);

				GAME_INFO("dist is {}", dist);

				if (*angle > 90)
				{
					gun_x = m_gameObject->m_transform->GetPosition().x - (dist * 2.0f);
				}
				else {
					gun_x = m_gameObject->m_transform->GetPosition().x + (dist * 2.0f);
				}
			}

			float end_x = gun_x + (laser_length * glm::cos(glm::radians(*angle)));
			float end_y = gun_y + (laser_length * glm::sin(glm::radians(*angle)));

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
			laserObj->m_transform->SetTrueScale(glm::vec3(laser_length, laser_size, 1.0f));
			laserObj->m_transform->SetRotation(*angle);

			//DamageEnemyInRange();

			//GLRenderer::GetInstance()->DrawDebug_Line(gun_x, gun_y, end_x, end_y, 1.0f, 0.0f, 0.0f);

			//get all collider in target layers
			Physic::RayHits Hits;
			for (std::string target : TargetLayers) {
				Physic::RayHits thislayerHit = PhySc->RaycastAll(Physic::Ray(gun_x, gun_y, end_x, end_y), PhySc->GetLayerFromString(target));
				Hits.insert(Hits.end(), thislayerHit.begin(), thislayerHit.end());
			}

			int winWidth = Graphic::Window::GetWidth() * Graphic::getCamera()->GetZoom();
			int winHeight = Graphic::Window::GetHeight() * Graphic::getCamera()->GetZoom();

			int particleIter = 0;
			for (Physic::RayHit h : Hits)
			{
				Enemy* enemy = h.collider->GetGameObject()->GetComponent<Enemy>();

				Transform* enemTransform = enemy->GetGameObject()->m_transform.get();

				if ((enemTransform->GetPosition().x > (Graphic::getCamera()->GetCampos().x + (winWidth / 2)))
					|| (enemTransform->GetPosition().x < (Graphic::getCamera()->GetCampos().x - (winWidth / 2)))
					|| (enemTransform->GetPosition().y > (Graphic::getCamera()->GetCampos().y + (winHeight / 2)))
					|| (enemTransform->GetPosition().y < (Graphic::getCamera()->GetCampos().y - (winHeight / 2))))
				{
					continue;
				}

				if (!enemy->GetGameObject()->Active()) 
				{
					continue;
				}

				if (enemy != nullptr)
				{
					//particle
					if (particleIter >= particles.size()) {
						particles[particleIter]->SetActive(true);
						particles[particleIter]->m_transform->SetPosition(enemTransform->GetPosition());
						particles[particleIter]->m_transform->SetRotation(*angle);
						++particleIter;
					}
					enemy->TakeDamage(weapon_damage);
				}
			}
		}
	}
	else if (Input::GetMouseUp(Input::MouseKeyCode::MOUSE_LEFT)) {
		stopParticles();
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

void LaserGun::getParticles() {
	if (!initparticle) {
		for (int i = 0; i < GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_LASER_HIT)->GetPoolSize(); ++i) {
			GameObject* p = GameController::GetInstance()->GetPool(POOL_TYPE::PTCL_LASER_HIT)->GetGameObject();
			p->SetActive(false);
			p->GetComponent<ParticleSystem>()->emitter->isEnabled = true;
			particles.push_back(p);
		}

		initparticle = true;
	}
}

void LaserGun::stopParticles() {
	for (GameObject* p : particles) {
		p->SetActive(false);
	}
}
