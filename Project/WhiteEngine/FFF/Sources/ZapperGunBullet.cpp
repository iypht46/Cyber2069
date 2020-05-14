#include "Weapon.hpp"
#include "Graphic/Camera.hpp"
#include "Core/Logger.hpp"
#include "Physic/PhysicScene.hpp"
#include "Graphic/GLRenderer.h"

#include "Enemy.hpp"

void ZapperGunBullet::OnUpdate(float dt)
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
		if (!isTriggerEnemy) {

			rb->SetVelocity(glm::vec3(0));
			m_gameObject->SetActive(false);
		}
	}

	if (isTriggerEnemy) {
		rb->SetVelocity(glm::vec3(0));
		zapDurationCount += dt;

		Zap(dt);
		
		if (zapDurationCount >= zapDuration) {

			enemRelease();
			isTriggerEnemy = false;
			m_gameObject->SetActive(false);
		}
	}
}

void ZapperGunBullet::OnAwake()
{
	rb = m_gameObject->GetComponent<Rigidbody>();
	ZappingSound = m_gameObject->GetComponent<SoundPlayer>();
	ZappingSound->SetSound(SoundPath("SFX_Zapper_Zapping"));
	cam = Graphic::getCamera();
}

void ZapperGunBullet::OnTriggerEnter(const Physic::Collision col) {
	//ENGINE_INFO("Bullet Hit " + col.m_otherCollider->GetGameObject()->Name);

	Enemy* enemy = col.m_otherCollider->GetGameObject()->GetComponent<Enemy>();
	if ((enemy != nullptr) && (enemy->GetGameObject()->Active()) && (!enemy->isZap())) {
		
		if (!isTriggerEnemy) {

			vector <Enemy*> tmp;
			vector <Transform*> transformTmp;
	
			Physic::PhysicScene* ps = Physic::PhysicScene::GetInstance();

			//get all collider in target layers
			//Physic::Colliders colliders;
			for (std::string target : TargetLayers) {
				Physic::Colliders layerColliders = ps->GetColliderLayer(target);
				colliders.insert(colliders.end(), layerColliders.begin(), layerColliders.end());
			}

			isTriggerEnemy = true;
			zapDurationCount = 0.0f;
			zapRateCount = 0.0f;

			target = enemy;

			if (target != nullptr) {

				target->SetAffectedByWeapon(true);
				target->SetGotZap(true);
				tmp.push_back(target);
				transformTmp.push_back(target->GetGameObject()->m_transform.get());

			}

			if (target != nullptr) {

				for (int i = 0; i < chainNumber - 1; i++)
				{
					target = FindTarget(target);

					if (target != nullptr)
					{
						target->SetAffectedByWeapon(true);
						target->SetGotZap(true);
						tmp.push_back(target);
						transformTmp.push_back(target->GetGameObject()->m_transform.get());
					}
					else
					{
						break;
					}
				}

				Targets = tmp;
				TargetTranform = transformTmp;
			}

			GAME_INFO("SIZE IS {}", Targets.size());
		}
	}

}

void ZapperGunBullet::OnCollisionEnter(const Physic::Collision col) {
	//ENGINE_INFO("Wall Hit: {}",m_gameObject->GetID());

	m_gameObject->SetActive(false);
}

Enemy* ZapperGunBullet::FindTarget(Enemy* e)
{
	float minDistance = -1;
	Enemy* nextTarget = nullptr;
	int index;

	for (int i = 0; i < colliders.size();i++) {

		Collider* c = colliders.at(i);

		if (c == nullptr) {
			continue;
		}

		if ((!c->GetGameObject()->Active()))
		{
			continue;
		}

		if (c->GetGameObject()->GetComponent<Enemy>()->isZap()) 
		{
			continue;
		}

		float distance = glm::length(c->GetGameObject()->m_transform->GetPosition() - e->GetGameObject()->m_transform->GetPosition());

		if (distance == 0) 
		{
			colliders.at(i) = nullptr;
			continue;
		}

		if (distance > zapDistance)
		{
			continue;
		}
		
		if (minDistance == -1) 
		{
			minDistance = distance;
			nextTarget = c->GetGameObject()->GetComponent<Enemy>();
			index = i;
		}
		else if(distance < minDistance) 
		{
			minDistance = distance;
			nextTarget = c->GetGameObject()->GetComponent<Enemy>();
			index = i;
		}
	}
	
	if (nextTarget != nullptr) 
	{
		colliders.at(index) = nullptr;
	}

	return nextTarget;
}

void ZapperGunBullet::Zap(float dt) {
	
	//zapRateCount += dt;

	for (int i = 0; i < Targets.size(); i++) {

		Enemy* e = Targets.at(i);

		if (e != nullptr) {

			Rigidbody* rb = e->GetGameObject()->GetComponent<Rigidbody>();

			rb->SetVelocity(glm::vec3(0.0f));

			if (zapDurationCount >= zapDuration) 
			{
				ZappingSound->PlaySound();
				e->TakeDamage(bulletDmg);
			}

			//e->GetGameObject()->m_transform->SetPosition(TargetTranform.at(i)->GetPosition());

			if (i < (Targets.size() - 1))
			{
				Enemy* n = Targets.at(i + 1);

				if (n != nullptr) {

					GLRenderer::GetInstance()->DrawDebug_Line(TargetTranform.at(i)->GetPosition().x, TargetTranform.at(i)->GetPosition().y
						, TargetTranform.at(i + 1)->GetPosition().x, TargetTranform.at(i + 1)->GetPosition().y
						, 0.0f, 0.0f, 1.0f);
				}
			}
		}
	}
}

void ZapperGunBullet::enemRelease() 
{
	for (int i = 0; i < Targets.size(); i++) {

		if (Targets.at(i) != nullptr) {
			if (!Targets.at(i)->GetGameObject()->GetComponent<HPsystem>()->isDead()) 
			{
				Targets.at(i)->SetAffectedByWeapon(false);
				Targets.at(i)->SetGotZap(false);
			}
			Targets.at(i)->SetState(EnemyState::Idle);
		}
	}
 }