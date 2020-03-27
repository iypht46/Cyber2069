#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Physic/PhysicScene.hpp"
#include "Input/Input.hpp"
#include "Core/EC/GameObject.hpp"

#include "HPsystem.hpp"

#include "Utility/ObjectPool.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

#define PI 3.14159265358979323846

class PlayerController : public BehaviourScript {
private://change later
	Physic::PhysicScene* ps;
protected:
	HPsystem* hpSystem;
	Transform* Gun;
	Rigidbody* rb;

	ObjectPool* MGbulletPool;
	
	//stats===============
	float max_stamina;

	float dashStamina;
	float jumpStamina;

	float max_move_speed;
	float move_speed;
	float dash_speed;
	float jump_speed;
	float dashTime;
	float delay;

	float camZoomSpeed;
	float camDelay;
	
	float camMaxZoom;
	float camMinZoom;

	float bullet_speed;
	float bullet_delay;

	float GunDistance;
	//======================

	//runtime var===========
	float stamina;
	float bullet_delay_count;
	float dashRemainingTime;
	float camDelay_count;

	bool inverseGun;
	bool running;
	bool jumping;
	bool falling;
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
	
	void mouseAim();

	void updateDirection();
	void move();
	void dash(float dt);
	void shoot(float dt);
	bool checkGround();

	float GetStamina();
	
	void cameraZoom(float dt);

	void assignPool(ObjectPool* pool);

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
			camZoomSpeed,
			camDelay,
			camMaxZoom,
			camMinZoom,
			GunDistance
		);
	}
};

CEREAL_REGISTER_TYPE(PlayerController);