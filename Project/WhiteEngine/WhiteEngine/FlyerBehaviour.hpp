#pragma once
#include "Core/EC/Components/BehaviourScript.h"

class FlyerBehaviour : public BehaviourScript {
public:
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};
