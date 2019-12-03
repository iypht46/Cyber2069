#pragma once

#include "BehaviourScript.h"
#include "Transform.hpp"
#include "Rigidbody.hpp"
#include "../GameObject.hpp"
#include "Utility/ObjectPool.h"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"

class EnemySpawner : public BehaviourScript 
{
protected:
	float SpawnDelay;
	float SpawnDelayCount;
	ObjectPool* FlyerPool;
public:
	void assignPool(ObjectPool* pool);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
