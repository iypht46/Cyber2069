#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Character.hpp"

class AirPatrol : public Character
{
private:
	float m_pointAX;
	float m_pointBX;
	float m_speed;
public:
	Transform* queen;
	Rigidbody* rb;
	AirPatrol();
	~AirPatrol();
	void Patrol();
	void SetPoint(float aX, float bX);
	void SetSpeed(float speed);
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};

