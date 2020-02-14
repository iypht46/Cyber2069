#pragma once
#include "Enemy.hpp"
#include "Utility/ObjectPool.h"
#include "AirFollowing.hpp"
#include "AirDash.hpp"
#include "AirPatrol.hpp"

class Flyer :public Enemy {
	AirFollowing* airFollow;
};

class Bomber :public Enemy {
	AirFollowing* airFollow;
	AirDash* airDash;
	//Explosion
	
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

class DeQueen :public Enemy {
	AirPatrol* airPatrol;
	ObjectPool* FlyerPool;
	ObjectPool* BomberPool;

	int PosX;
	int PosY;
	float SpawnDelay;
	float SpawnDelayCount;

	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
public:
	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);
};