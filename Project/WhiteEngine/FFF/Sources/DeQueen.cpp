#include "EnemyBehaviours.h"
#include "GameController.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"
#include "Core/Logger.hpp"

#include "ItemDrop.hpp"

void DeQueen::OnAwake() {
	airPatrol = GetGameObject()->GetComponent<AirPatrol>();
	QueenSound = GetGameObject()->GetComponent<SoundPlayer>();
	
	SpawnDelay = 0.2f;
	SpawnDelayCount = SpawnDelay;

	FlyerPool = GameController::GetInstance()->GetPool(POOL_TYPE::ENEMY_FLYER);
	BomberPool = GameController::GetInstance()->GetPool(POOL_TYPE::ENEMY_BOMBER);
	ItemPool = GameController::GetInstance()->GetPool(POOL_TYPE::ITEM_DROP);

	QueenSound = GetGameObject()->GetComponent<SoundPlayer>();

	SoundCounter = 0.30f;

	QueenSound->SetSound(SoundPath("SFX_Queen_SpawningEnemy"));
	QueenSound->SetLoop(false);

	Enemy::OnAwake();
}

void DeQueen::OnEnable() 
{
	Enemy::OnEnable();
}

void DeQueen::SetStats(float Speed, float HP, float SpawnDelay, float unlockchance, float healvalue) {
	airPatrol->SetSpeed(Speed);
	hpSystem->SetMaxHP(HP);
	this->SpawnDelay = SpawnDelay;
	this->HealValue = healvalue;
	this->ItemUnlockDropChance = unlockchance;
}

void DeQueen::OnUpdate(float dt) {
	
	//no need to detect target
	//Enemy::OnUpdate(dt);

	airPatrol->Patrol();

	if ((m_gameObject->m_transform->GetPosition().x > DespawnPosX) || (m_gameObject->m_transform->GetPosition().x < -DespawnPosX)) 
	{
		m_gameObject->SetActive(false);
	}

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

				flyer->GetComponent<Enemy>()->SetTarget(GameController::GetInstance()->GetPlayer()->m_transform.get());
			}
		}
		else {
			GameObject* bomber = BomberPool->GetInactiveObject();
			if (bomber != nullptr)
			{
				bomber->SetActive(true);

				bomber->m_transform->SetPosition(glm::vec3(spawnPosX, spawnPosY, 1.0f));

				bomber->GetComponent<Enemy>()->SetTarget(GameController::GetInstance()->GetPlayer()->m_transform.get());
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

void DeQueen::SpawnItem() 
{
	GameObject* item = ItemPool->GetGameObject();

	if (item != nullptr) {

		item->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(0));
		item->m_transform->SetPosition(m_gameObject->m_transform->GetPosition());

		item->SetActive(true);

		if (!GameController::GetInstance()->GetGameObject()->GetComponent<EquipmentManager>()->isAllUnlock())
		{
			int randChance = (rand() % 100) + 1;

			if (randChance < ItemUnlockDropChance) 
			{
				item->GetComponent<ItemDrop>()->SetType(Drop_Type::Unlock);
			}
			else 
			{
				item->GetComponent<ItemDrop>()->SetType(Drop_Type::Heal);
				item->GetComponent<ItemDrop>()->SetHealValue(HealValue);
			}
		}
		else 
		{
			item->GetComponent<ItemDrop>()->SetType(Drop_Type::Heal);
			item->GetComponent<ItemDrop>()->SetHealValue(HealValue);
		}
		
	}
	else {
		ENGINE_ERROR("{} Can't spawn Item", *m_gameObject);
	}
}