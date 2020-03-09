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

struct EnemyPreset;
struct EnemyAmplifier;

class EnemySpawner : public BehaviourScript 
{
protected:
	float SpawnRateCount = -1;
	float SpawnRate;

	int SpawnType;

	int x1, x2, y1, y2;
	ObjectPool* EnemyPool;
	
	EnemyPreset* SpawnPreset;
	EnemyAmplifier* SpawnAmplifier;

public:
	GameObject* SpawnEnemy(float rangeX, float rangeY);

	void SetSpawnRate(float value);
	void SetSpawnRange(float x1, float y1, float x2, float y2);
	void SetSpawnType(int type);
	int GetType() { return this->SpawnType; }

	void updateSpawner();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
