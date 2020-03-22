#pragma once
#include "Enemy.hpp"
#include "Utility/ObjectPool.h"
#include "Utility/Event.h" 
#include "AirFollowing.hpp"
#include "AirDash.hpp"
#include "AirPatrol.hpp"

#include <memory>

#include <cereal/types/polymorphic.hpp>

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

	Flyer() {}
	~Flyer() {}

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
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	AirDash* airDash;
	EventSystem OnExplode;

public:
	EnemyState state = EnemyState::Idle;

	Bomber() {}
	~Bomber() {}

	void Init(Transform* player);
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

	//EnemyState state = EnemyState::Idle;
	
	DeQueen() {}
	~DeQueen() {}

	void Init();
	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);
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
