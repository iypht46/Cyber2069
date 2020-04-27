#include "EnemySpawner.hpp"

void EnemySpawner::OnUpdate(float dt)
{
	//ENGINE_INFO("intv = {}/{}", SpawnRateCount, 1.0f / SpawnRate);
	SpawnRateCount += dt;

	if (SpawnRateCount > 1.0f / SpawnRate)
	{
		SpawnRateCount = 0;

		SpawnEnemy();
	}
}

GameObject* EnemySpawner::SpawnEnemy() {
	if (Spawning) {
		if (SpawnRateCount <= 0)
		{
			SpawnRateCount = SpawnRate;

			float randPosX, randPosY;

			if (x2 > x1) {
				randPosX = (rand() % (x2 - x1 + 1)) + x1;
			}
			else {
				randPosX = (rand() % (x1 - x2 + 1)) + x2;
			}

			if (y2 > y1) {
				randPosY = (rand() % (y2 - y1 + 1)) + y1;
			}
			else {
				randPosY = (rand() % (y1 - y2 + 1)) + y2;
			}

			return SpawnEnemy(randPosX, randPosY);
		}
	}

	return nullptr;
}

GameObject* EnemySpawner::SpawnEnemy(float posX,float posY)
{
	if (SpawnAmplifier != nullptr) {
		if (EnemyPool != nullptr) {
			GameObject* enemy = EnemyPool->GetInactiveObject();
			if (enemy != nullptr)
			{
				enemy->GetComponent<Enemy>()->SetTarget(EnemyTarget->m_transform.get());

				enemy->SetActive(true);
				enemy->GetComponent<HPsystem>()->ResetHP();
				enemy->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(0));

				enemy->m_transform->SetPosition(glm::vec3(posX, posY, 1.0f));

				switch (SpawnType) {
				case POOL_TYPE::ENEMY_FLYER:
					enemy->GetComponent<Flyer>()->SetStats(
						SpawnAmplifier->FlyerSpeed,
						SpawnAmplifier->FlyerHP,
						SpawnAmplifier->FlyerDmg
						);
					break;
				case POOL_TYPE::ENEMY_BOMBER:
					enemy->GetComponent<Bomber>()->SetStats(
						SpawnAmplifier->BomberSpeed,
						SpawnAmplifier->BomberHP,
						SpawnAmplifier->BomberDmg,
						SpawnAmplifier->BomberAimTime,
						SpawnAmplifier->BomberDashSpeed,
						SpawnAmplifier->BomberExplodeDMG,
						SpawnAmplifier->BomberExplodeRadius
						);
					break;
				case POOL_TYPE::ENEMY_QUEEN:
					enemy->GetComponent<DeQueen>()->SetStats(
						SpawnAmplifier->QueenSpeed,
						SpawnAmplifier->QueenHP,
						SpawnAmplifier->QueenSpawnDelay
						);
					break;
				case POOL_TYPE::ENEMY_TANK:
					enemy->GetComponent<Tank>()->SetStats(
						SpawnAmplifier->TankSpeed,
						SpawnAmplifier->TankHP
						);
					break;
				case POOL_TYPE::ENEMY_CHARGER:
					enemy->GetComponent<Charger>()->SetStats(
						SpawnAmplifier->ChargerSpeed,
						SpawnAmplifier->ChargerHP,
						SpawnAmplifier->ChargerDashPauseTime,
						SpawnAmplifier->ChargerDashSpeed,
						SpawnAmplifier->ChragerDashDamage
						);
					break;
				case POOL_TYPE::ENEMY_SPITTER:
					enemy->GetComponent<Spitter>()->SetStats(
						SpawnAmplifier->SpitterSpeed,
						SpawnAmplifier->SpitterHP,
						SpawnAmplifier->SpitterFireRate
						);
					break;
				default:
					break;
				}

				return enemy;
			}
		}
		else {
			ENGINE_WARN("No enemy Pool assigned");
		}
	}
	else {
		ENGINE_WARN("No enemy amplifier assigned");
	}

	return nullptr;
}

void EnemySpawner::SetSpawnRange(float x1, float y1, float x2, float y2) {
	this->x1 = x1; this->x2 = x2; this->y1 = y1; this->y2 = y2;
}

void EnemySpawner::SetSpawnRate(float value) {
	this->SpawnRate = value;
	SpawnRateCount = 0;
}

void EnemySpawner::SetSpawnType(int type) {
	this->SpawnType = type;
	EnemyPool = GameController::GetInstance()->GetPool(type);
}

void EnemySpawner::updateSpawner() {

	if (SpawnPreset != nullptr) {
		switch (SpawnType)
		{
		case POOL_TYPE::ENEMY_FLYER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->FlyerRatio;
			break;
		case POOL_TYPE::ENEMY_BOMBER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->BomberRatio;
			break;
		case POOL_TYPE::ENEMY_QUEEN:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->QueenRatio;
			break;
		case POOL_TYPE::ENEMY_TANK:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->TankRatio;
			break;
		case POOL_TYPE::ENEMY_CHARGER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->ChargerRatio;
			break;
		case POOL_TYPE::ENEMY_SPITTER:
			SpawnRate = SpawnAmplifier->EnemySpawnRate * SpawnPreset->SpitterRatio;
			break;
		default:
			break;
		}
	}
	else {
		ENGINE_WARN("No enemy preset assigned");
	}
	
	SpawnRateCount = SpawnRate;
}