#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"
#include "Utility/ObjectPool.h"
#include "GameController.hpp"

#include "EnemyBehaviours.h"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>


struct EnemyPreset;
struct EnemyAmplifier;

class EnemySpawner : public BehaviourScript 
{
protected:
	float SpawnRate;
	//spawn area
	int x1, x2, y1, y2;
	//enemy type
	int SpawnType;

	float SpawnRateCount = -1;
	ObjectPool* EnemyPool;

public:
	EnemyPreset* SpawnPreset = nullptr;
	EnemyAmplifier* SpawnAmplifier = nullptr;

	GameObject* SpawnEnemy(float rangeX, float rangeY);

	void SetSpawnRate(float value);
	void SetSpawnRange(float x1, float y1, float x2, float y2);
	void SetSpawnType(int type);
	int GetType() { return this->SpawnType; }

	void updateSpawner();

	EnemySpawner() {}
	~EnemySpawner() {}

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			SpawnRate,
			x1, x2, y1, y2,
			SpawnType
			);
	}
};

CEREAL_REGISTER_TYPE(EnemySpawner);
