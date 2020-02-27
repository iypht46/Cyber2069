#pragma once
#include "Equipment.hpp"
#include "Utility/ObjectPool.h"

class Weapon : public Equipment 
{
protected:
	float bullet_speed;
	float weapon_firerate;
	float weapon_damage;

	float weapon_delay_count;

	GameObject* weaponObj;
	ObjectPool* BulletPool;
	
	float* angle;

	float angle_deg;
	float angle_rad;
public:
	virtual void Modify(GameObject* obj) = 0;
	virtual void GameTimeBehaviour(float dt) = 0;

	GameObject* GetWeapon() { return this->weaponObj; }
	
	float SetBulletSpeed(float value) { this->bullet_speed = value; }
	float SetWeaponFireRate(float value) { this->weapon_firerate = value; }
	float SetWeaponDamage(float value) { this->weapon_damage = value; }

	void AssignAngle(float* angle) { this->angle = angle; }
	void AssignPool(ObjectPool* ObjPool) { this->BulletPool = ObjPool; }
};