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
	Transform* bomber;
	HPsystem* hp;
	
public:
	Explosion();
	~Explosion();
	void Explode();
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};

