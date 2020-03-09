#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Physic/PhysicScene.hpp"

class GroundPatrol : public BehaviourScript
{
private:
	float m_stoppingDistance;
	float m_speed;
	float delay_timer;
	float MaxDelay;
	Transform* thisTransform;
	Rigidbody* rb;
	Physic::PhysicScene* ps;
public:
	GroundPatrol();
	~GroundPatrol();
	void Init();
	void SetStopDis(float val);
	void SetSpeed(float val);
	void SetMaxDelay(float val);
	void Patrol(float dt);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

