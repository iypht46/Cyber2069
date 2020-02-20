#pragma once
#include "Enemy.hpp"
#include "Utility/ObjectPool.h"
#include "AirFollowing.hpp"
#include "AirDash.hpp"
#include "AirPatrol.hpp"

enum EnemyState
{
	Idle = 0,
	Chase,
	Active,
};


class Flyer :public Enemy {
private:
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
public:
	EnemyState state = EnemyState::Idle;

	void Init(Transform* player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

};


class Bomber :public Enemy {
private:
	float DashTriggerRadius;
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	AirDash* airDash;
	//Explosion expl

public:
	EnemyState state = EnemyState::Idle;

	void Init(Transform* player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

class DeQueen :public Enemy {
private:
	float PosX;
	float PosY;
	float SpawnDelay;
	float SpawnDelayCount;
protected:
	AirPatrol* airPatrol;
	ObjectPool* FlyerPool;
	ObjectPool* BomberPool;
public:

	//EnemyState state = EnemyState::Idle;
	
	void Init();
	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);
	void SetSpawnDelay(int time);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};