#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Character.hpp"

class AirDash : public Character
{
private: 
	float timer;
	bool dashState;
	bool dashEnd;
	bool targetLocked;
protected:
	glm::vec3 m_target;
	Transform* bomber;
	Rigidbody* rb;
	float m_dashSpeed;
	float m_aimTime;
	float m_aimSpeed;
	float m_angle;
public:
	AirDash();
	void Init();
	void SetDashSpeed(float value);
	void SetAimTime(float value);
	void SetAimSpeed(float value);
	void Dash(float dt);
	void TargetLock(glm::vec3 pos);
	bool DashEnd();
	void Reset();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};

