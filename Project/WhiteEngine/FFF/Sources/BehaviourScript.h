#pragma once
#include "Core/EC/Components/Component.hpp"
struct Physic::Collision;

class BehaviourScript :public Component {
public:
	BehaviourScript();
	virtual void SetGameObject(GameObject* obj);
	virtual void OnAwake() = 0;
	virtual void OnEnable() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnFixedUpdate(float dt) = 0;
	virtual void OnDisable() = 0;
	inline virtual void OnTriggerEnter(Physic::Collision col) {}
	inline virtual void OnTriggerExit(Physic::Collision col) {}
	inline virtual void OnCollisionEnter(Physic::Collision col) {}
	inline virtual void OnCollisionExit(Physic::Collision col) {}
};