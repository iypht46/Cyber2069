#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

#include <cereal/types/polymorphic.hpp>

class AirFollowing : public BehaviourScript
{
private:
	std::shared_ptr<Rigidbody> rb;
	std::shared_ptr<Transform> t;
	std::shared_ptr<Transform> m_target;
protected:
	float m_speed;
	float rotAngle;
	float rotRate;
public:
	AirFollowing();
	void SetPlayer(std::shared_ptr<Transform>);
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

	//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			m_speed,
			rotAngle,
			rotRate
		);
	}
};

CEREAL_REGISTER_TYPE(AirFollowing);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BehaviourScript, AirFollowing);
