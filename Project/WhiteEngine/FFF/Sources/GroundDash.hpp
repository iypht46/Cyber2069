#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

#include "GroundPatrol.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class GroundDash : public BehaviourScript
{
private:
	float pauseTimer;
	float dashDestinationX;	//target.x + exceedingdistance
	bool dashing;

	bool targetLocked;
protected:
	Transform* thisTransform = nullptr;
	Transform* m_target = nullptr;
	Rigidbody* rb = nullptr;
	GroundPatrol* patrol = nullptr;

	float m_dashSpeed = 700.0f;
	float m_exceedingDashDistance = 100.0f;
	float m_minDashDistance = 200.0f;
	float m_pauseTime = 0.25f;
public:
	GroundDash();
	~GroundDash();
	void SetDashSpeed(float val);
	void SetDashDis(float val);
	void SetPauseTime(float val);
	void LockTarget(Transform* target);
	bool DashEnd();
	bool dashEnd;
	void Dash(float dt);
	virtual void OnAwake();

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			m_dashSpeed,
			m_exceedingDashDistance,
			m_minDashDistance,
			m_pauseTime
			);
	}
};

