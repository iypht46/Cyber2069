#include "EnemyBehaviours.h"
#include "GameController.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"
#include "Core/Logger.hpp"

void DeQueen::OnAwake() {
	airPatrol = GetGameObject()->GetComponent<AirPatrol>();
	
	SpawnDelay = 0.2f;
	SpawnDelayCount = SpawnDelay;

	FlyerPool = GameController::GetInstance()->GetPool(POOL_TYPE::ENEMY_FLYER);
	BomberPool = GameController::GetInstance()->GetPool(POOL_TYPE::ENEMY_BOMBER);

	QueenSound = GetGameObject()->GetComponent<SoundPlayer>();

	SoundCounter = 0.30f;

	QueenSound->CreateSoundPlayer();
	QueenSound->SetSound(SoundPath("SFX_Queen_SpawningEnemy"));
	QueenSound->SetLoop(false);

	Enemy::OnAwake();
}

void DeQueen::SetStats(float Speed, float HP, float SpawnDelay) {
	airPatrol->SetSpeed(Speed);
	hpSystem->SetMaxHP(HP);
	this->SpawnDelay = SpawnDelay;
}

void DeQueen::OnUpdate(float dt) {
	
	//no need to detect target
	//Enemy::OnUpdate(dt);

	airPatrol->Patrol();

	SpawnDelayCount -= dt;
	if (SpawnDelayCount <= 0)
	{
		SpawnDelayCount = SpawnDelay;
		int randSpawn = rand() % 2;
		int spawnPosX = airPatrol->queen->GetPosition().x;
		int spawnPosY = airPatrol->queen->GetPosition().y - 100;
		if (randSpawn == 0) {
			//std::shared_ptr<GameObject> flyer = FlyerPool->GetInactiveObject();
			GameObject* flyer = FlyerPool->GetInactiveObject();
			if (flyer != nullptr)
			{
				flyer->SetActive(true);

				flyer->m_transform->SetPosition(glm::vec3(spawnPosX, spawnPosY, 1.0f));
			}
		}
		else {
			GameObject* bomber = BomberPool->GetInactiveObject();
			if (bomber != nullptr)
			{
				bomber->SetActive(true);

				bomber->m_transform->SetPosition(glm::vec3(spawnPosX, spawnPosY, 1.0f));
			}
		}

		SoundCounter -= dt;
		if (SoundCounter <= 0.0f) {
			QueenSound->PlaySound();
			SoundCounter = 0.30f;
		}
		
	}

}

void DeQueen::OnFixedUpdate(float dt) {

}

//void DeQueen::assignFlyPool(ObjectPool* pool) {
//	this->FlyerPool = pool;
//}
//
//void DeQueen::assignBombPool(ObjectPool* pool) {
//	this->BomberPool = pool;
//}

void DeQueen::SetSpawnDelay(int time) {
	this->SpawnDelay = time;
}