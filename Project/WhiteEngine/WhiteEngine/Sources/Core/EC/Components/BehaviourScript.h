#pragma once
#include "Component.hpp"
class BehaviourScript :public Component {
	virtual void OnAwake() = 0;
	virtual void OnEnable() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnFixedUpdate(float dt) = 0;
	virtual void OnDisable() = 0;
};