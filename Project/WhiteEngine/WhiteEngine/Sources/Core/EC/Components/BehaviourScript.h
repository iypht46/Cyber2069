#pragma once
#include "Core/EC/Components/Component.hpp"

//Forward Declaration
namespace Physic{ struct Collision; }

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
	virtual void OnTriggerEnter(const Physic::Collision* col);
	virtual void OnTriggerExit(const Physic::Collision* col);
	virtual void OnCollisionEnter(const Physic::Collision* col);
	virtual void OnCollisionExit(const Physic::Collision* col);
};