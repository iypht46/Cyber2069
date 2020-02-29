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
		int randSpawn = rand() % 2;
		int randPosX = (rand() % (Graphic::Window::GetWidth() * 2)) - Graphic::Window::GetWidth();
		int randPosY = (rand() % (Graphic::Window::GetHeight() * 2)) - Graphic::Window::GetHeight();
		if (randSpawn == 0) {
			GameObject* flyer = FlyerPool->GetInactiveObject();
			if (flyer != nullptr)
			{
				flyer->SetActive(true);

				flyer->m_transform.SetPosition(glm::vec3(0.0f, 25.0f, 1.0f));
			}
		}
		else {
			GameObject* bomber = BomberPool->GetInactiveObject();
			if (bomber != nullptr)
			{
				bomber->SetActive(true);

				bomber->m_transform.SetPosition(glm::vec3(randPosX, randPosY, 1.0f));
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

void EnemySpawner::assignFlyPool(ObjectPool* pool)
{
	this->FlyerPool = pool;
}

void EnemySpawner::assignBombPool(ObjectPool* pool) {
	this->BomberPool = pool;
}