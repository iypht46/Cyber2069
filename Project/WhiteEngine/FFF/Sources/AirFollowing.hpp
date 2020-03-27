#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

#include <cereal/types/polymorphic.hpp>

class AirFollowing : public BehaviourScript
{
private:
	Rigidbody* rb;
	Transform* t;
	Transform* m_target;
protected:
	float m_speed;
	float rotAngle;
	float rotRate;
public:
	AirFollowing();
	~AirFollowing() {}
	void SetPlayer(Transform*);
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
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			m_speed,
			rotAngle,
			rotRate
		);
	}
};

CEREAL_REGISTER_TYPE(AirFollowing);
