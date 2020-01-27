#pragma once
#include "BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

#define MOVE_SPEED_FLYER 100.0f

class FlyerBehaviour : public BehaviourScript {
protected:
	Transform* player;
	Transform* flyer;
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
