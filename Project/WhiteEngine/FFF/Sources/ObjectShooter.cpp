#include "ObjectShooter.h"
#include "GameController.hpp"

ObjectShooter::ObjectShooter() {
	BulletObjectTypeAsInt = POOL_TYPE::BULLET_FUME;
}

void ObjectShooter::OnAwake() {
	BulletPool = GameController::GetInstance()->GetPool(BulletObjectTypeAsInt);
	weapon_delay_count = 1.0f / firerate;
}

void ObjectShooter::OnFixedUpdate(float dt) {
	weapon_delay_count += dt;
}

bool ObjectShooter::CooledDown() {
	return weapon_delay_count >= (1.0f / firerate);
}

GameObject* ObjectShooter::Shoot(Transform* target) {
	if (target != nullptr) {

		GameObject* bullet = BulletPool->GetGameObject();

		if (bullet != nullptr) {
			bullet->SetActive(true);

			glm::vec3 direction;
			if (bulletSpawnPos != nullptr) {
				direction = glm::normalize(target->GetPosition() - bulletSpawnPos->GetPosition());
				bullet->m_transform->SetPosition(bulletSpawnPos->GetPosition());
			}
			else {
				direction = glm::normalize(target->GetPosition() - m_gameObject->m_transform->GetPosition());
				bullet->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());
			}
			glm::vec3 speed = direction * bullet_speed;


			bullet->m_transform->SetRotation(glm::degrees(glm::atan(direction.y / direction.x)));

			bullet->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(speed.x, speed.y, 0.0f));

			weapon_delay_count = 0.0f;
		}

		return bullet;
	}

	return nullptr;
}