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
#include "Artifact.hpp"

#include "Enemy.hpp"
#include "Character.hpp"


#include "Utility/ObjectPool.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

#define PI 3.14159265358979323846

class PlayerController : public Character {
protected:
	HPsystem* hpSystem = nullptr;
	Rigidbody* rb = nullptr;

	ObjectPool* MGbulletPool = nullptr;

	vector<Equipment*> Equipments;
	vector<Weapon*> Weapons;

	Weapon* weapon = nullptr;
	Transform* weaponTranform = nullptr;

	float yLimit = -1000.0f;
	
	//stats===============
	float max_stamina = 50000000.0f;

	float dashStamina = 5.0f;
	float jumpStamina = 5.0f;
	float staminaRegenRate = 1.0f;

	float max_move_speed = 200.0f;
	float move_speed = 200.0f;
	float dash_speed = 750.0f;
	float jump_speed = 300.0f;
	float dashTime = 0.35f;


	float camZoomInSpeed = 0.01f;
	float camZoomOutSpeed = 0.005f;
	float camZoomInDelay = 0.0f;
	float camSmall = 1.5f;
	float camLarge = 0.65f;

	float GunDistance = 0.45f;
	//======================

	//runtime var===========
	float stamina;
	float dashRemainingTime;
	float camDelay_count;
	float delay;

	bool inverseGun = false;
	bool running = false;
	bool jumping = false;
	bool falling = false;
	bool onGround = false;
	bool Dash = false;
	bool setDashAnim = false;
	
	glm::vec2 direction;
	glm::vec2 dashDirection;
	glm::vec3 velo;

	float angle_deg, angle_rad;

public:
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
	void assignWeapon();
	void assignWeapon(Weapon* wp);

	void AddEquipment(Equipment* e);
	void AddEquipment(GameObject* obj);
	void RemoveWeapon(int index);
	void RemoveEquipment(int index);

	void ModifyFromEquipment();
	void RevertEquipment();

	void MultiplyMoveSpeed(float value);

	vector<Weapon*> GetWeapons() { return Weapons; }
	vector<Equipment*> GetEquipments() { return Equipments; }

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
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
			cereal::base_class<Character>(this),
			max_stamina,
			dashStamina,
			jumpStamina,
			max_move_speed,
			dash_speed,
			jump_speed,
			dashTime,
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