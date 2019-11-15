#pragma once
#include "BehaviourScript.h"
#include "Transform.hpp"
#include "Rigidbody.hpp"
#include "Animator.hpp"
#include "Input/Input.hpp"
#include "../GameObject.hpp"

#define PI 3.14159265358979323846

class PlayerController : public BehaviourScript {
protected:
	Transform* Gun;
	Rigidbody* rb;
	
	float move_speed;
	float dash_speed;
	float jump_speed;
	float dashTime;
	float dashRemainingTime;
	float delay;

	float GunDistance;
	bool inverseGun;
	bool running;
	bool jumping;
	bool Dash;
	bool setDashAnim;
	
	glm::vec2 direction;
	glm::vec2 dashDirection;
	glm::vec3 velo;

	float angle_deg, angle_rad;

public:
	PlayerController();
	
	void mouseAim();

	void updateDirection();
	void move();
	void dash(float dt);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};