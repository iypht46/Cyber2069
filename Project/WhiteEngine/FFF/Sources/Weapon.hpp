#pragma once
#include "Equipment.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"
#include "Utility/ObjectPool.h"

#include "Physic/PhysicScene.hpp"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"
#include "Enemy.hpp"

#include <cereal/types/polymorphic.hpp>

class Weapon : public Equipment , public BehaviourScript
{
protected:
	float bullet_speed;
	float weapon_firerate;
	float weapon_damage;

	float weapon_delay_count = 0.0f;

	GameObject* weaponObj = nullptr;
	ObjectPool* BulletPool = nullptr;
	
	float* angle;

	float angle_deg;
	float angle_rad;

	glm::vec2 weapon_scale;
	glm::vec2 bullet_scale;
public:
	virtual void Modify(GameObject* obj) = 0;
	virtual void GameTimeBehaviour(float dt) = 0;

	GameObject* GetWeapon() { return this->m_gameObject; }
	
	void SetBulletSpeed(float value) { this->bullet_speed = value; }
	void SetWeaponFireRate(float value) { this->weapon_firerate = value; }
	void SetWeaponDamage(float value) { this->weapon_damage = value; }

	void MultiplyweaponBulletSpeed(float value) { this->bullet_speed = this->bullet_speed * value; }
	void MultiplyWeaponFireRate(float value) { this->weapon_firerate = this->weapon_firerate / value; }
	void MultiplyWeaponDamage(float value) { this->weapon_damage = this->weapon_damage * value; }
	virtual void MultiplyWeaponAmplifier(float value) = 0;

	glm::vec2 GetWeaponScale() { return weapon_scale; }

	void AssignAngle(float* angle) { this->angle = angle; }
	void AssignPool(ObjectPool* ObjPool) { this->BulletPool = ObjPool; }

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			bullet_speed,
			weapon_firerate,
			weapon_damage
		);	
	}
};

CEREAL_REGISTER_TYPE(Weapon);

class MachineGun : public Weapon {
public:
	MachineGun();
	void Modify(GameObject* obj);
	void GameTimeBehaviour(float dt);
	void MultiplyWeaponAmplifier(float value);

	virtual void OnAwake();
};

class MachineGunBullet : public BehaviourScript {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg;

public:
	void SetDamage(float dmg) { this->bulletDmg = dmg; }

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;
};

class LaserGun : public Weapon {
private:
	GameObject* laser = nullptr;
	float laser_length;
	float laser_size;
	float laser_duration;
	float laser_duration_count = 0.0f;


	glm::vec2 Lstart;
	glm::vec2 Lend;

	glm::vec2 gunPos;
	glm::vec2 endPos;
public:
	LaserGun();
	void Modify(GameObject* obj);
	void GameTimeBehaviour(float dt);
	void AssignLaserObj(GameObject* obj) { this->laser = obj; }
	void MultiplyWeaponAmplifier(float value);

	/*void DamageEnemyInRange();
	bool isInRange(Collider* col);*/
	
	virtual void OnAwake();
	virtual void OnDisable();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Weapon>(this),
			laser_size
		);
	}
};

CEREAL_REGISTER_TYPE(LaserGun);

class GrenadeLauncher : public Weapon {
private:
	float grenade_radius;
public:
	GrenadeLauncher();
	void Modify(GameObject* obj);
	void GameTimeBehaviour(float dt);
	void MultiplyWeaponAmplifier(float value);

	virtual void OnAwake();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Weapon>(this),
			grenade_radius
			);
	}
};

CEREAL_REGISTER_TYPE(GrenadeLauncher);

class GrenadeLauncherBullet : public BehaviourScript {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg;

	float radius;
	float scaleX;

public:
	void SetDamage(float dmg) { this->bulletDmg = dmg; }
	void SerRadius(float radius) { this->radius = radius; }

	void Explode();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;
};

class ZapperGun : public Weapon {
private:
	int chainNumber;
	float zapDistance;
	float zapDuration;
	float zapRate;
public:
	ZapperGun();
	void Modify(GameObject* obj);
	void GameTimeBehaviour(float dt);
	void MultiplyWeaponAmplifier(float value);
	
	virtual void OnAwake();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Weapon>(this),
			chainNumber,
			zapDistance,
			zapDuration,
			zapRate
		);
	}
};

CEREAL_REGISTER_TYPE(ZapperGun);

class ZapperGunBullet : public BehaviourScript {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg;
	int chainNumber;
	float zapDistance;
	float zapDuration;
	float zapRate;

	float zapDurationCount = 0.0f;
	float zapRateCount = 0.0f;

	bool isTriggerEnemy = false;

	Enemy* target;

	vector <Enemy*> Targets;
	vector <Transform*> TargetTranform;
	Physic::Colliders colliders;

public:
	void SetDamage(float dmg) { this->bulletDmg = dmg; }
	void SetChainNumber(float n) { this->chainNumber = n; }
	void SetZapDistance(float d) { this->zapDistance = d; }
	void SetZapDuration(float d) { this->zapDuration = d; }
	void SetZapRate(float r) { this->zapRate = r; }

	Enemy* FindTarget(Enemy* e);
	void Zap(float dt);
	void enemRelease();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;
};

class BlackholeGun : public Weapon {
private:
	float bullet_Duration;
	float bullet_Radius;
	float bullet_ToCenterSpeed;
public:
	BlackholeGun();
	void Modify(GameObject* obj);
	void GameTimeBehaviour(float dt);
	void MultiplyWeaponAmplifier(float value);

	virtual void OnAwake();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Weapon>(this),
			bullet_Duration,
			bullet_Radius,
			bullet_ToCenterSpeed
		);
	}
};

CEREAL_REGISTER_TYPE(BlackholeGun);

class BlackholeGunBullet : public BehaviourScript {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg;
	float Duration;
	float DurationCount = 0;
	float Radius;
	float ToCenterSpeed;

	float Dot_count = 0.0f;

	bool isTriggerEnemy = false;

public:
	void SetDamage(float dmg) { this->bulletDmg = dmg; }
	void SetDuration(float duration) { this->Duration = duration; }
	void SetRadius(float radius) { this->Radius = radius; }
	void SetToCenterSpeed(float spd) { this->ToCenterSpeed = spd; }

	void DragEnemy(float dt);
	void ReleaseEnemy();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;
};