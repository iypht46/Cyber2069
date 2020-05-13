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

	float dashDelay = 1.0f;
	float dashDelayTimer = 0;

protected:
	AirFollowing* airFollow;
	AirDash* airDash;
public:

	Flyer() {}
	~Flyer() {}

	void SetStats(float Speed, float rotateSpeed, float aimtime, float dashspeed, float HP, float Dmg);

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
	Explosion* explosion = nullptr;

public:
	EnemyState state = EnemyState::Idle;

	Bomber();
	~Bomber() {}

public:
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

	float ItemUnlockDropChance;
	float HealValue;

	float DespawnPosX = 2000.0f;

	float SpawnDelayCount;
	float SoundCounter;
protected:
	AirPatrol* airPatrol;
	ObjectPool* FlyerPool;
	ObjectPool* BomberPool;
	ObjectPool* ItemPool;
	SoundPlayer* QueenSound;
public:

	void SetStats(float Speed, float HP, float SpawnDelay, float unlockchance, float healvalue);

	void SpawnItem();

	//void assignFlyPool(ObjectPool* pool);
	//void assignBombPool(ObjectPool* pool);
	void SetSpawnDelay(int time);
	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

	DeQueen() {}
	~DeQueen() {}
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
	int SoundCounter;
public:
	void SetStats(float Speed, float HP);

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
	bool dashingTmp = false;

public:
	float DashTriggerRangeY = 150.0f;

	void SetStats(float Speed, float HP, float DashPauseTime, float DashSpeed, float Dmg);

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


	void Spit();

public:

	void SetStats(float Speed, float HP, float FireRate);

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

class Cocoon : public Enemy {
public:
	virtual void OnUpdate(float dt);
	
	//serialization
public:
	template <class Archive>
	void serialize(Archive& archive) {
		archive(cereal::base_class<Enemy>(this));
	}
};
CEREAL_REGISTER_TYPE(Cocoon);