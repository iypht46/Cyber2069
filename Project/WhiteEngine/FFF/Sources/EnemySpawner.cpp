#include "EnemySpawner.hpp"


void EnemySpawner::OnStart()
{
}

void EnemySpawner::OnUpdate(float dt)
{
	if (GameController::GetInstance()->isChangeDifficulty()) 
	{
		updateSpawner();
	}

	SpawnRateCount -= dt;

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

		SpawnEnemy(randPosX, randPosY);
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

GameObject* EnemySpawner::SpawnEnemy(float posX,float posY)
{
	GameObject* enemy = EnemyPool->GetInactiveObject();
	if (enemy != nullptr)
	{
		enemy->SetActive(true);

		enemy->m_transform.SetPosition(glm::vec3(posX, posY, 1.0f));

		switch (SpawnType) {
		case POOL_TYPE::ENEMY_FLYER:
			enemy->GetComponent<Flyer>()->SetStats(SpawnAmplifier->FlyerSpeed, SpawnAmplifier->FlyerHP, SpawnAmplifier->FlyerDmg);
			break;
		case POOL_TYPE::ENEMY_BOMBER:
			enemy->GetComponent<Bomber>()->SetStats(SpawnAmplifier->BomberSpeed, SpawnAmplifier->BomberHP, SpawnAmplifier->BomberDmg
													, SpawnAmplifier->BomberAimTime, SpawnAmplifier->BomberDashSpeed
													, SpawnAmplifier->BomberExplodeDMG, SpawnAmplifier->BomberExplodeRadius);
			break;
		default:
			break;
		}

		return enemy;
	}

	return nullptr;
}

void EnemySpawner::SetSpawnRange(float x1, float y1, float x2, float y2) {
	this->x1 = x1; this->x2 = x2; this->y1 = y1; this->y2 = y2;
}

void EnemySpawner::SetSpawnRate(float value) {
	this->SpawnRate = value;
	SpawnRateCount = SpawnRate;
}

void EnemySpawner::SetSpawnType(int type) {
	this->SpawnType = type;
	EnemyPool = GameController::GetInstance()->GetPool(type);
}

void EnemySpawner::updateSpawner() {

	SpawnAmplifier = GameController::GetInstance()->GetCurrAmplifier();
	SpawnPreset = GameController::GetInstance()->GetCurrPreset();

	switch (SpawnType)
	{
	case POOL_TYPE::ENEMY_FLYER:
		SpawnRate = SpawnAmplifier->EnemySpawnRate / SpawnPreset->FlyerRatio;
		break;
	case POOL_TYPE::ENEMY_BOMBER:
		SpawnRate = SpawnAmplifier->EnemySpawnRate / SpawnPreset->BomberRatio;
		break;
	default:
		break;
	}
	
	SpawnRateCount = SpawnRate;
}