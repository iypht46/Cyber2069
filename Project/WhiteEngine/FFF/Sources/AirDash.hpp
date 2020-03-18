#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

#include <cereal/types/polymorphic.hpp>

class AirDash : public BehaviourScript
{
private: 
	float timer;
	float maxExplodeTime;
	bool dashState;
protected:
	std::shared_ptr<Transform> m_target;
	std::shared_ptr<Transform> bomber;
	std::shared_ptr<Rigidbody> rb;

	float m_dashSpeed;
	float m_aimTime;
	float m_aimSpeed;

	float m_angle;
	float m_explodeCountDown;
public:
	AirDash();
	void SetPlayer(std::shared_ptr<Transform> player);
	void SetDashSpeed(float value);
	void SetAimTime(float value);
	void SetAimSpeed(float value);
	void Dash(float dt);
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();

	//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			maxExplodeTime,
			m_dashSpeed,
			m_aimTime,
			m_aimSpeed
		);
	}
};

CEREAL_REGISTER_TYPE(AirDash);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BehaviourScript, AirDash);
