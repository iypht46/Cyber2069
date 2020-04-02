#pragma once
#include "Enemy.hpp"
#include "Utility/ObjectPool.h"
#include "AirFollowing.hpp"
#include "AirDash.hpp"
#include "AirPatrol.hpp"
#include "Explosion.hpp"
#include "GroundPatrol.hpp"
#include "GroundDash.hpp"


class Flyer :public Enemy {
private:
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	
public:

	void Init(Transform* player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

	void SetStats(float Speed, float HP, float Dmg);

};


class Bomber :public Enemy {
private:
	float DashTriggerRadius;
	float ExplodeTriggerRadius;
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	AirDash* airDash;
	Explosion* explosion;

public:
	void Init(Transform* player);
	void SetStats(float Speed, float HP, float Dmg, float AimTime, float DashSpeed, float ExplodeDmg, float ExplodeRadius);

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
	
	void Init();
	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);
	void SetSpawnDelay(int time);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

class Charger : public Enemy {
private:
	Rigidbody* rigidbody;
protected:
	GroundPatrol* groundPatrol;
	GroundDash* groundDash;
public:

	void Init(Transform* player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};