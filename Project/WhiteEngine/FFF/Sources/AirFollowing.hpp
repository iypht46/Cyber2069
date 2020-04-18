#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Character.hpp"
#include "Enemy.hpp"

#include <cereal/types/polymorphic.hpp>

class AirFollowing : public BehaviourScript
{
private:
	Character* e;
	Rigidbody* rb;
	Transform* t;
	Transform* m_target;
protected:
	float m_speed = 300;
	float rotAngle = 15;
	float rotRate = 2;
public:
	AirFollowing();
	~AirFollowing() {}
	void SetPlayer(Transform*);
	void SetFlySpeed(float value);
	void SetRotAngle(float value);
	void SetRotRate(float value);
	void FollowPlayer(float dt);
	virtual void OnAwake();

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
