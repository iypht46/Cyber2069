#pragma once
#include <vector>
#include "Core/EC/Components/BehaviourScript.h"
#include "HPsystem.hpp"

class Enemy :BehaviourScript {
protected:
	HPsystem* hpSystem;

	Transform* target;
	float targetDetectionRange;
	bool foundTarget;

	bool isDead = false;

	//events on take damage (funct*)
	//events on dead (funct*)

	void OnTakeDamage();
	void OnDead();
public:
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt) = 0;

	void SetTarget(Transform*);
	void TakeDamage(float);
};