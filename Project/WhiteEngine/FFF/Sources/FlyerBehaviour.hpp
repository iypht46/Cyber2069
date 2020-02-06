#pragma once
#include "BehaviourScript.h"
#include "AirFollowing.hpp"
#include "AirDash.hpp" //For testing

class FlyerBehaviour : public BehaviourScript {

public:
	AirFollowing* af;
	AirDash* ad; //For Testing
	FlyerBehaviour();
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
