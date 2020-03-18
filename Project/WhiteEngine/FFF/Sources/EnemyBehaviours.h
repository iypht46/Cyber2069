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
	std::shared_ptr<Rigidbody> rigidbody;
protected:
	std::shared_ptr<AirFollowing> airFollow;
public:
	EnemyState state = EnemyState::Idle;

	void Init(std::shared_ptr<Transform> player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

	//serialization
private:
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
	std::shared_ptr<Rigidbody> rigidbody;
protected:
	std::shared_ptr<AirFollowing> airFollow;
	std::shared_ptr<AirDash> airDash;
	EventSystem OnExplode;

public:
	EnemyState state = EnemyState::Idle;

	void Init(std::shared_ptr<Transform> player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

	//serialization
private:
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
	std::shared_ptr<AirPatrol> airPatrol;
	ObjectPool* FlyerPool;
	ObjectPool* BomberPool;
public:

	//EnemyState state = EnemyState::Idle;
	
	void Init();
	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

	//serialization
private:
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
