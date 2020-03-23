#pragma once
#include <vector>
#include "Core/EC/Components/BehaviourScript.h"
#include "HPsystem.hpp"
#include "Core/EC/Components/Animator.hpp"

#include "Character.hpp"

enum EnemyState
{
	Idle = 0,
	Chase,
	Active,
};

class Enemy :public Character {
protected:
	HPsystem* hpSystem;
	Animator* animator;
	Transform* target;
	bool foundTarget;

	bool isDead = false;

	bool gotBlackHole = false;


	EnemyState state = EnemyState::Idle;

	//events on take damage (funct*)
	//events on dead (funct*)

	float CollideDamage = 1.0f;

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

	float GetCollideDamage() { return CollideDamage; }
	void SetGotBlackHole(bool bh) { this->gotBlackHole = bh; }
	void SetState(EnemyState state) { this->state = state; }
};