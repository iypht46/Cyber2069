#pragma once
#include "Core/EC/Components/Component.hpp"

//Forward Declaration
namespace Physic{ struct Collision; }

class BehaviourScript :public Component {
public:
	BehaviourScript();
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnFixedUpdate(float dt) = 0;
	virtual void OnDisable();
	virtual void OnTriggerEnter(const Physic::Collision col);
	virtual void OnTriggerStay(const Physic::Collision col);
	virtual void OnTriggerExit(const Physic::Collision col);
	virtual void OnCollisionEnter(const Physic::Collision col);
	virtual void OnCollisionStay(const Physic::Collision col);
	virtual void OnCollisionExit(const Physic::Collision col);
	virtual ~BehaviourScript() {};
};