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
#include "ObjectShooter.h"
#include "Core/EC/Components/SoundPlayer.hpp"

#include <memory>

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>


class Flyer :public Enemy {
private:
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	SoundPlayer* FlyerSound;
	
public:

	Flyer() {}
	~Flyer() {}

	void SetStats(float Speed, float HP, float Dmg);

	virtual void OnAwake() override;
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
	float DashTriggerRadius = 1000.0f;
	float ExplodeTriggerRadius = 300.0f;
	Rigidbody* rigidbody = nullptr;
protected:
	AirFollowing* airFollow = nullptr;
	AirDash* airDash = nullptr;
	SoundPlayer* BomberSound;

public:
	EnemyState state = EnemyState::Idle;

	Bomber() {}
	~Bomber() {}

	Explosion* explosion = nullptr;

public:
	//void Init(Transform* player);
	void SetStats(float Speed, float HP, float Dmg, float AimTime, float DashSpeed, float ExplodeDmg, float ExplodeRadius);

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Enemy>(this),
			DashTriggerRadius,
			ExplodeTriggerRadius
		);
	}
};
CEREAL_REGISTER_TYPE(Bomber);

class DeQueen :public Enemy {
private:
	float SpawnDelay;

	float SpawnDelayCount;
protected:
	AirPatrol* airPatrol;
	ObjectPool* FlyerPool;
	ObjectPool* BomberPool;
	SoundPlayer* QueenSound;
public:
	
	DeQueen() {}
	~DeQueen() {}

	void assignFlyPool(ObjectPool* pool);
	void assignBombPool(ObjectPool* pool);
	void SetSpawnDelay(int time);
	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Enemy>(this),
			SpawnDelay
		);
	}
};
CEREAL_REGISTER_TYPE(DeQueen);

class Tank : public Enemy {
private:
	Rigidbody* rigidbody;
	GroundPatrol* groundPatrol;
	SoundPlayer* TankSound;
public:
	virtual void OnAwake();
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
CEREAL_REGISTER_TYPE(Tank);

class Charger : public Enemy {
private:
	Rigidbody* rigidbody;
	GroundPatrol* groundPatrol;
	GroundDash* groundDash;
	SoundPlayer* ChargerSound;
public:
	float DashTriggerRangeY = 150.0f;

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Enemy>(this),
			DashTriggerRangeY
			);
	}
};
CEREAL_REGISTER_TYPE(Charger);

class Spitter : public Enemy {
private:
	Rigidbody* rigidbody;
	GroundPatrol* groundPatrol;
	ObjectShooter* shooting;
	SoundPlayer* SpitterSound;
public:

	virtual void OnAwake();
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
CEREAL_REGISTER_TYPE(Spitter);