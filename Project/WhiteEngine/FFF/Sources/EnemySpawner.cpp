#include "EnemySpawner.hpp"


void EnemySpawner::OnStart()
{
	SpawnDelay = 1;
	SpawnDelayCount = SpawnDelay;
}

void EnemySpawner::OnUpdate(float dt)
{
	SpawnDelayCount -= dt;
	if (SpawnDelayCount <= 0)
	{
		SpawnDelayCount = SpawnDelay;
		int randSpawn = rand() % 101;

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
		
		if (randSpawn <= SpawnRate) {
			GameObject* enemy = EnemyPool->GetInactiveObject();
			if (enemy != nullptr)
			{
				enemy->SetActive(true);

				enemy->m_transform.SetPosition(glm::vec3(randPosX, randPosY, 1.0f));
			}
		}

	}

}

void EnemySpawner::OnFixedUpdate(float dt) {

}

void EnemySpawner::OnAwake() {

}

void EnemySpawner::OnEnable() {

}

void EnemySpawner::OnDisable() {

}

void EnemySpawner::SetSpawnDelay(float value) {
	this->SpawnDelay = value;
}

void EnemySpawner::SetSpawnRange(float x1, float y1, float x2, float y2) {
	this->x1 = x1; this->x2 = x2; this->y1 = y1; this->y2 = y2;
}

void EnemySpawner::SetSpawnRate(float value) {
	this->SpawnRate = value;
}

void EnemySpawner::SetSpawnType(int type) {
	this->SpawnType = type;
	EnemyPool = GameController::GetInstance()->GetPool(type);
}
