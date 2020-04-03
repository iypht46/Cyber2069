#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Physic/PhysicScene.hpp"
#include "Input/Input.hpp"
#include "Core/EC/GameObject.hpp"

#include "HPsystem.hpp"
#include "Weapon.hpp"

#include "Enemy.hpp"
#include "Character.hpp"


#include "Utility/ObjectPool.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

#define PI 3.14159265358979323846

class PlayerController : public Character {
private://change later
	Physic::PhysicScene* ps;
protected:
	HPsystem* hpSystem = nullptr;
	Rigidbody* rb = nullptr;

	ObjectPool* MGbulletPool = nullptr;

	vector<Equipment*> Equipments;
	vector<Weapon*> Weapons;

	Weapon* weapon = nullptr;
	Transform* weaponTranform = nullptr;
	
	//stats===============
	float max_stamina;

	float dashStamina;
	float jumpStamina;
	float staminaRegenRate;

	float max_move_speed;
	float move_speed;
	float dash_speed;
	float jump_speed;
	float dashTime;
	float delay;

	float camZoomInSpeed;
	float camZoomOutSpeed;
	float camZoomInDelay;
	float camSmall;
	float camLarge;

	float GunDistance;
	//======================

	//runtime var===========
	float stamina;
	float dashRemainingTime;
	float camDelay_count;

	bool inverseGun;
	bool running;
	bool jumping;
	bool falling;
	bool onGround;
	bool Dash;
	bool setDashAnim;
	
	glm::vec2 direction;
	glm::vec2 dashDirection;
	glm::vec3 velo;

	float angle_deg, angle_rad;

public:
	void PSSet(Physic::PhysicScene* ps) { this->ps = ps; }
	PlayerController();
	~PlayerController() {}
	
	void DebugInput();

	void mouseAim();

	void updateDirection();
	void move();
	void dash(float dt);
	bool checkGround();

	float GetStamina() { return this->stamina; }
	float GetMaxStamina() { return this->max_stamina; }
	
	void cameraZoom(float dt);

	void assignPool(ObjectPool* pool);
	void assignWeapon(Weapon* wp);

	void AddEquipment(Equipment* e);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	virtual void OnCollisionEnter(const Physic::Collision) override;
	virtual void OnCollisionStay(const Physic::Collision) override;
	virtual void OnCollisionExit(const Physic::Collision) override;
	virtual void OnTriggerEnter(const Physic::Collision) override;
	virtual void OnTriggerStay(const Physic::Collision) override;
	virtual void OnTriggerExit(const Physic::Collision) override;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			max_stamina,
			dashStamina,
			jumpStamina,
			max_move_speed,
			dash_speed,
			jump_speed,
			dashTime,
			delay,
			camZoomInSpeed,
			camZoomOutSpeed,
			camZoomInDelay,
			camSmall,
			camLarge,
			GunDistance
			);
	}
};

CEREAL_REGISTER_TYPE(PlayerController);