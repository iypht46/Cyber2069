#include "EnemySpawner.hpp"

#include "FlyerBehaviour.hpp"

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
		GameObject* flyer = FlyerPool->GetInactiveObject();
		if (flyer != nullptr) 
		{
			int testPosX = -(Graphic::Window::GetWidth() / 2);
			int testPosY = Graphic::Window::GetHeight() / 2;
			//int randPosX = (rand() % (Graphic::Window::GetWidth() * 2)) - Graphic::Window::GetWidth();
			//int randPosY = (rand() % (Graphic::Window::GetHeight() * 2)) - Graphic::Window::GetHeight();

			flyer->SetActive(true);
			//
			//flyer->GetComponent<FlyerBehaviour>()->hp = 3;
			//

			flyer->m_transform.SetPosition(glm::vec3(testPosX, testPosY, 1.0f));
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

void EnemySpawner::assignPool(ObjectPool* pool)
{
	this->FlyerPool = pool;
}