#pragma once
#include "BehaviourScript.h"
#include "Transform.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

#define MOVE_SPEED_FLYER 100.0f
#define ROT_ANGLE 15.0f
#define ROT_RATE 1.5f

class FlyerBehaviour : public BehaviourScript {
protected:
	Transform* player;
	Transform* flyer;
	float flyerSpeed = 100.0f;
	float rotAngle = 15.0f;
	float rotRate = 1.5f;
public:
	FlyerBehaviour();
	void SetPlayer(Transform&);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
