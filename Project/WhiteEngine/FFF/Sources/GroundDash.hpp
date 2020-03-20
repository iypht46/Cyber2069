#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

class GroundDash : public BehaviourScript
{
private:
	float dashTimer;
	float endTimer;
	float dir;
	bool dashing;

	bool targetLocked;
protected:
	Transform* m_target;
	Transform* thisTransform;
	Rigidbody* rb;
	float m_dashSpeed;
	float m_dashDistance;
	float m_pauseTime;
	float m_dashEndTime;
public:
	GroundDash();
	~GroundDash();
	void Init();
	void SetDashSpeed(float val);
	void SetDashDis(float val);
	void SetPauseTime(float val);
	void LockTarget(Transform* target);
	bool DashEnd();
	bool dashEnd;
	void Dash(float dt);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

