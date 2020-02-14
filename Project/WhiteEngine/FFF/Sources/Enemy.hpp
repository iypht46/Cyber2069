#pragma once
#include <vector>
#include "Core/EC/Components/BehaviourScript.h"
#include "HPsystem.hpp"
#include "Core/EC/Components/Animator.hpp"

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
};