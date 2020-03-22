#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"
#include "Utility/ObjectPool.h"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"


class EnemySpawner : public BehaviourScript 
{
protected:
	float SpawnDelay;
	float SpawnDelayCount;
	ObjectPool* FlyerPool;
	ObjectPool* BomberPool;
public:
	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);

	EnemySpawner() {}
	~EnemySpawner() {}

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
