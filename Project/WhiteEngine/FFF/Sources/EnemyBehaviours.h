#pragma once
#include "Enemy.hpp"
#include "Utility/ObjectPool.h"
#include "Utility/Event.h" 
#include "AirFollowing.hpp"
#include "AirDash.hpp"
#include "AirPatrol.hpp"
#include "Explosion.hpp"
#include "GroundPatrol.hpp"
#include "GroundDash.hpp"

#include <memory>

#include <cereal/types/polymorphic.hpp>


class Flyer :public Enemy {
private:
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	
public:

	Flyer() {}
	~Flyer() {}

	void SetStats(float Speed, float HP, float Dmg);

	//change this to awake
	void Init(Transform* player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);


	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Enemy>(this)
		);
	}
};

CEREAL_REGISTER_TYPE(Flyer);

class Bomber :public Enemy {
private:
	float DashTriggerRadius;
	float ExplodeTriggerRadius;
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	AirDash* airDash;

public:
	EnemyState state = EnemyState::Idle;

	Bomber() {}
	~Bomber() {}

	Explosion* explosion;

public:
	void Init(Transform* player);
	void SetStats(float Speed, float HP, float Dmg, float AimTime, float DashSpeed, float ExplodeDmg, float ExplodeRadius);

	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Enemy>(this),
			DashTriggerRadius
		);
	}
};

CEREAL_REGISTER_TYPE(Bomber);

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
	
	DeQueen() {}
	~DeQueen() {}

	void Init();
	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);
	void SetSpawnDelay(int time);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

		//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Enemy>(this),
			PosX,
			PosY,
			SpawnDelay
		);
	}
};

CEREAL_REGISTER_TYPE(DeQueen);


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
