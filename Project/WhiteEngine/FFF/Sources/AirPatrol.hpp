#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
class AirPatrol : public BehaviourScript
{
private:
	Transform* m_pointA;
	Transform* m_pointB;
	Transform* queen;
	Rigidbody* rb;
	float m_speed;
public:
	AirPatrol();
	~AirPatrol();
	void Patrol(float dt);
	void SetPoint(float aX, float aY, float bX, float bY);
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};

