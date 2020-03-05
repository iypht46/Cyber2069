#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"
#include "Utility/ObjectPool.h"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"

#include "GameController.hpp"

#include "EnemyBehaviours.h"

class EnemySpawner : public BehaviourScript 
{
protected:
	float SpawnRateCount;
	float SpawnRate;

	float SpawnEnemySpeed;
	float SpawnEnemyHP;
	float SpawnEnemyDamage;
	
	float SpawnEnemyAimTime;
	float SpawnEnemyDashSpeed;

	float SpawnEnemyExplodeDMG;
	float SpawnEnemyExplodeRadius;


	int SpawnType;

	int x1, x2, y1, y2;
	ObjectPool* EnemyPool;


public:
	GameObject* SpawnEnemy(float rangeX, float rangeY);

	void SetSpawnRate(float value);
	void SetSpawnRange(float x1, float y1, float x2, float y2);
	void SetSpawnType(int type);

	void SetSpawnEnemySpeed(float value);
	void SetSpawnEnemyHP(float value);
	void SetSpawnEnemyDamage(float value);

	void SetSpawnEnemyBasicStats(float Speed, float HP, float Dmg);
	void SetSpawnEnemyDashStats(float AimTime, float DashSpeed);
	void SetSpawnEnemyExplodeStats(float ExplodeDmg, float ExplodeRadius);

	int GetType() { return this->SpawnType; }

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
