#pragma once
#include "BehaviourScript.h"
#include "Transform.hpp"
#include "Input/Input.hpp"
#include "../GameObject.hpp"

#define PI 3.14159265358979323846

class PlayerController : public BehaviourScript {
protected:
	Transform* Gun;
	float GunDistance;
	bool inverseGun;

	float angle_deg, angle_rad;

public:
	PlayerController();

	void mouseAim();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
