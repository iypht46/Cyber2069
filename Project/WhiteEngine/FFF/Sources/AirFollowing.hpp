#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

class AirFollowing : public BehaviourScript
{
protected:
	Transform* m_target;
	Transform* flyer;
	float m_speed;
	float rotAngle;
	float rotRate;
public:
	AirFollowing();
	void SetPlayer(Transform&);
	void SetFlySpeed(float value);
	void SetRotAngle(float value);
	void SetRotRate(float value);
	void FollowPlayer(float dt);
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};

