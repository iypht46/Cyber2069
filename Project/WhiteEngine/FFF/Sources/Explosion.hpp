#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Physic/PhysicScene.hpp"
#include "HPsystem.hpp"

class Explosion : public BehaviourScript
{
private:
	float m_damage;
	float m_radius;
	Transform* thisTransform;
	Physic::PhysicScene* ps;
	Physic::Layer targetLayer;
	
public:
	Explosion();
	~Explosion();
	void Init();
	void Explode();
	void SetLayer(std::string layer);
	void SetDamage(float val);
	void SetRadius(float val);
	virtual void OnAwake();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);

};

