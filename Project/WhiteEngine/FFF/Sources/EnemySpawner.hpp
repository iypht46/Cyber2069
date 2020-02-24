#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"
#include "Utility/ObjectPool.h"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"

#include "GameController.hpp"


class EnemySpawner : public BehaviourScript 
{
protected:
	float SpawnDelay;
	float SpawnDelayCount;
	float SpawnRate;
	int SpawnType;

	int x1, x2, y1, y2;
	ObjectPool* EnemyPool;
public:
	void SetSpawnDelay(float value);
	void SetSpawnRate(float value);
	void SetSpawnRange(float x1, float y1, float x2, float y2);
	void SetSpawnType(int type);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
