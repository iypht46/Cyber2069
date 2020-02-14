#pragma once
#include "Enemy.hpp"
#include "AirFollowing.hpp"
#include "AirDash.hpp"

#include "Core/EC/Components/Rigidbody.hpp"

enum EnemyState
{
	Idle = 0,
	Chase,
	Active,
};

class Flyer :public Enemy {
private:
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;

public:
	EnemyState state = EnemyState::Idle;

	void Init(Transform* player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

class Bomber :public Enemy {
private:
	float DashTriggerRadius;
	Rigidbody* rigidbody;
protected:
	AirFollowing* airFollow;
	AirDash* airDash;
	//Explosion expl
	
public:
	EnemyState state = EnemyState::Idle;
	
	void Init(Transform* player);
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

class DeQueen :public Enemy {
	
};