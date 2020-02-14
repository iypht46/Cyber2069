#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Enemy.hpp"
#include "AirFollowing.hpp"

class FlyerBehaviour : public BehaviourScript {

public:
	FlyerBehaviour();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};
