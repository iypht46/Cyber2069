#pragma once
#include <vector>
#include <memory>
#include "Core/EC/Components/BehaviourScript.h"
#include "HPsystem.hpp"
#include "Core/EC/Components/Animator.hpp"

#include <cereal/types/polymorphic.hpp>

class Enemy :public BehaviourScript {
protected:
	HPsystem* hpSystem;
	Animator* animator;
	Transform* target;
	bool foundTarget;

	bool isDead = false;

	//events on take damage (funct*)
	//events on dead (funct*)

	void OnTakeDamage();
	void OnDead();
public:
	float targetDetectionRange = 0;

	void Init();
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) override;
	virtual void OnFixedUpdate(float dt) = 0;

	void SetTarget(Transform*);
	void TakeDamage(float);

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			targetDetectionRange
		);
	}
};

CEREAL_REGISTER_TYPE(Enemy);