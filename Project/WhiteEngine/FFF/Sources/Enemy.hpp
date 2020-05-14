#pragma once
#include <vector>
#include <memory>
#include "Core/EC/Components/BehaviourScript.h"
#include "HPsystem.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Character.hpp"

#include <cereal/types/base_class.hpp>
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
	SoundPlayer* sp;
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
	float baseScore = 1.0f;
	float CollideDamage = 0.0f;
	float targetDetectionRange = 2000.0f;

	virtual void OnAwake() override;
	virtual void OnUpdate(float dt) override;

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
			cereal::base_class<Character>(this),
			targetDetectionRange,
			baseScore,
			CollideDamage
		);
	}
};

CEREAL_REGISTER_TYPE(Enemy);

