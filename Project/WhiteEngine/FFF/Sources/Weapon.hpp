#pragma once
#include "Equipment.hpp"
#include <memory>
#include <set>
#include <string>

#include <cereal/types/string.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

enum WEAPON_TYPE {
	WEAPON_MACHINEGUN = 0,
	WEAPON_LASER,
	WEAPON_GRENADELAUNCHER,
	WEAPON_ZAPPER,
	WEAPON_BLACKHOLE,
};

class Weapon : public Equipment , public BehaviourScript
{
protected:
	float bullet_speed;
	float weapon_firerate;
	float weapon_damage;

	GameObject* weaponObj = nullptr;
	ObjectPool* BulletPool = nullptr;
	
	float* angle;

	float angle_deg;
	float angle_rad;

	glm::vec2 weapon_scale;
	glm::vec2 bullet_scale;

	float weapon_delay_count = 0.0f;


public:
	std::set<std::string> TargetLayers;

	virtual void Modify() = 0;
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
			cereal::base_class<Equipment>(this),
			cereal::base_class<BehaviourScript>(this),
			TargetLayers,
			bullet_speed,
			weapon_firerate,
			weapon_damage
		);	
	}
};
CEREAL_REGISTER_TYPE(Weapon);

class Bullet :public BehaviourScript {
public:
	std::set<std::string> TargetLayers;

	bool isTarget(Physic::Layer);
	bool isTarget(std::string);

	Bullet();
	~Bullet();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			TargetLayers
			);
	}
};
CEREAL_REGISTER_TYPE(Bullet);

//Machine Gun ===========================================================================================
class MachineGun : public Weapon {
public:
	MachineGun();
	void Modify();
	void GameTimeBehaviour(float dt);
	void MultiplyWeaponAmplifier(float value);

	virtual void OnAwake();

//serialization
public:
	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Weapon>(this)
			);
	}
};
CEREAL_REGISTER_TYPE(MachineGun);

class MachineGunBullet : public Bullet {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg = 1.0f;

public:
	void SetDamage(float dmg) { this->bulletDmg = dmg; }

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Bullet>(this),
			bulletDmg
			);
	}
};

CEREAL_REGISTER_TYPE(MachineGunBullet);
//-------------------------------------------------------------------------------------------------------

//Laser Gun =============================================================================================
class LaserGun : public Weapon {
private:
	std::shared_ptr <GameObject> laserObj = nullptr;
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
	void Modify();
	void GameTimeBehaviour(float dt);
	void AssignLaserObj(std::shared_ptr <GameObject> obj) { this->laserObj = obj; }
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
			laserObj,
			laser_size
		);
	}
};
CEREAL_REGISTER_TYPE(LaserGun);
//-------------------------------------------------------------------------------------------------------


//Grenade Launcher ======================================================================================
class GrenadeLauncher : public Weapon {
private:
	float grenade_radius;
public:
	GrenadeLauncher();
	void Modify();
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

class GrenadeLauncherBullet : public Bullet {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg = 1;

	float radius = 1000.0f;
	float scaleX = 1.0f;

public:
	void SetDamage(float dmg) { this->bulletDmg = dmg; }
	void SerRadius(float radius) { this->radius = radius; }

	void Explode();

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Bullet>(this),
			bulletDmg,
			radius,
			scaleX
			);
	}
};

CEREAL_REGISTER_TYPE(GrenadeLauncherBullet);
//-------------------------------------------------------------------------------------------------------

//Zapper Gun ============================================================================================
class ZapperGun : public Weapon {
private:
	int chainNumber;
	float zapDistance;
	float zapDuration;
	float zapRate;
public:
	ZapperGun();
	void Modify();
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

class ZapperGunBullet : public Bullet {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg = 1;
	int chainNumber = 3;
	float zapDistance = 150;
	float zapDuration = 0.5f;
	float zapRate = 1;

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
	virtual void OnUpdate(float dt);
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Bullet>(this),
			bulletDmg,
			chainNumber,
			zapDistance,
			zapDuration,
			zapRate
			);
	}
};

CEREAL_REGISTER_TYPE(ZapperGunBullet);
//-------------------------------------------------------------------------------------------------------

//Blackhole Gun =========================================================================================
class BlackholeGun : public Weapon {
private:
	float bullet_Duration;
	float bullet_Radius;
	float bullet_ToCenterSpeed;
public:
	BlackholeGun();
	void Modify();
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

class BlackholeGunBullet : public Bullet {
protected:
	Graphic::CameraObject* cam;

	Rigidbody* rb;
	float bulletDmg = 1.0f;
	float Duration = 2.0f;
	float Radius = 200.0f;
	float ToCenterSpeed = 10.0f;

	float Dot_count = 0.0f;

	bool isTriggerEnemy = false;
	float DurationCount = 0;

public:
	void SetDamage(float dmg) { this->bulletDmg = dmg; }
	void SetDuration(float duration) { this->Duration = duration; }
	void SetRadius(float radius) { this->Radius = radius; }
	void SetToCenterSpeed(float spd) { this->ToCenterSpeed = spd; }

	void DragEnemy(float dt);
	void ReleaseEnemy();

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Bullet>(this),
			bulletDmg,
			Duration,
			Radius,
			ToCenterSpeed
			);
	}
};
CEREAL_REGISTER_TYPE(BlackholeGunBullet);
//-------------------------------------------------------------------------------------------------------