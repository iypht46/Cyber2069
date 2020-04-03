#pragma once
#include <vector>
#include <memory>
#include "Core/EC/Components/BehaviourScript.h"
#include "HPsystem.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Character.hpp"

#include <cereal/types/polymorphic.hpp>

enum EnemyState
{
	Idle = 0,
	Chase,
	Active,
	Dash,
	Reset
};

class Enemy :public Character {
protected:
	HPsystem* hpSystem;
	Animator* animator;
	Transform* target;
	bool foundTarget;

	bool isDead = false;

	bool affectedByWeapon = false;
	bool GotZap = false;


	EnemyState state = EnemyState::Idle;

	//events on take damage (funct*)
	//events on dead (funct*)

	void OnTakeDamage();
	void OnDead();
public:
	float CollideDamage = 1.0f;
	float targetDetectionRange = 0;

	void Init();
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) override;
	virtual void OnFixedUpdate(float dt) = 0;

	void SetTarget(Transform*);
	void TakeDamage(float);

	float GetCollideDamage() { return CollideDamage; }
	void SetAffectedByWeapon(bool b) { this->affectedByWeapon = b; }
	void SetState(EnemyState state) { this->state = state; }

	bool isZap() { return GotZap; }
	void SetGotZap(bool z) { GotZap = z; }

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			targetDetectionRange,
			CollideDamage
		);
	}
};

CEREAL_REGISTER_TYPE(Enemy);

