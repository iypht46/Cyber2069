#pragma once
#include <vector>
#include <memory>
#include "Core/EC/Components/BehaviourScript.h"
#include "HPsystem.hpp"
#include "Core/EC/Components/Animator.hpp"

#include <cereal/types/polymorphic.hpp>

class Enemy :public BehaviourScript {
protected:
	std::shared_ptr<HPsystem> hpSystem;
	std::shared_ptr<Animator> animator;
	std::shared_ptr<Transform> target;
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

	void SetTarget(std::shared_ptr<Transform>);
	void TakeDamage(float);

//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			targetDetectionRange
		);
	}
};

CEREAL_REGISTER_TYPE(Enemy);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BehaviourScript, Enemy);