#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Physic/PhysicScene.hpp"
#include "Input/Input.hpp"
#include "Core/EC/GameObject.hpp"

#include "Utility/ObjectPool.h"

#define PI 3.14159265358979323846

class PlayerController : public BehaviourScript {
private://change later
	Physic::PhysicScene* ps;
protected:
	Transform* Gun;
	Rigidbody* rb;

	ObjectPool* MGbulletPool;
	
	float max_move_speed;
	float move_speed;
	float dash_speed;
	float jump_speed;
	float dashTime;
	float dashRemainingTime;
	float delay;
	float m_drag;

	float camZoomSpeed;
	float camDelay;
	float camDelay_count;

	float bullet_speed;
	float bullet_delay;
	float bullet_delay_count;

	float GunDistance;
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
	
	void mouseAim();

	void updateDirection();
	void move();
	void dash(float dt);
	void shoot(float dt);

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
};
