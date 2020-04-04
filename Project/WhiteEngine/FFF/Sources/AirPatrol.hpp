#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Character.hpp"

#include <cereal/types/polymorphic.hpp>

class AirPatrol : public BehaviourScript
{
private:
	float m_pointAX;
	float m_pointBX;
	float m_speed;
public:
	Transform* queen;
	Rigidbody* rb;
	AirPatrol();
	~AirPatrol() {}
	void Patrol();
	void SetPoint(float aX, float bX);
	void SetSpeed(float speed);
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			m_pointAX,
			m_pointBX,
			m_speed
		);
	}
};

CEREAL_REGISTER_TYPE(AirPatrol);
