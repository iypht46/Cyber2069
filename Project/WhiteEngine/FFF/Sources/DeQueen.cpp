#include "EnemyBehaviours.h"
#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"
#include "Core/Logger.hpp"

void DeQueen::Init() {
	airPatrol = m_gameObject->GetComponent<AirPatrol>();
	PosX = -(Graphic::Window::GetWidth() / 2);
	PosY = Graphic::Window::GetHeight() / 2;
	
	airPatrol->SetPoint(PosX, -PosX);
	SpawnDelay = 0.2f;
	SpawnDelayCount = SpawnDelay;

	Enemy::Init();
}


void DeQueen::OnStart() {

}

void DeQueen::OnUpdate(float dt) {
	
	Enemy::OnUpdate(dt);

	airPatrol->Patrol();

	SpawnDelayCount -= dt;
	if (SpawnDelayCount <= 0)
	{
		SpawnDelayCount = SpawnDelay;
		int randSpawn = rand() % 2;
		int spawnPosX = airPatrol->queen->GetPosition().x;
		int spawnPosY = airPatrol->queen->GetPosition().y - 100;
		if (randSpawn == 0) {
			GameObject* flyer = FlyerPool->GetInactiveObject();
			if (flyer != nullptr)
			{
				flyer->SetActive(true);

				flyer->m_transform.SetPosition(glm::vec3(spawnPosX, spawnPosY, 1.0f));
			}
		}
		else {
			GameObject* bomber = BomberPool->GetInactiveObject();
			if (bomber != nullptr)
			{
				bomber->SetActive(true);

				bomber->m_transform.SetPosition(glm::vec3(spawnPosX, spawnPosY, 1.0f));
			}
		}
		
	}

}

void DeQueen::OnFixedUpdate(float dt) {

}

void DeQueen::assignFlyPool(ObjectPool* pool) {
	this->FlyerPool = pool;
}

void DeQueen::assignBombPool(ObjectPool* pool) {
	this->BomberPool = pool;
}

void DeQueen::SetSpawnDelay(int time) {
	this->SpawnDelay = time;
}