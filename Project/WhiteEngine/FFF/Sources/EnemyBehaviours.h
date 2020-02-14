#pragma once
#include "Enemy.hpp"
#include "AirFollowing.hpp"
#include "AirDash.hpp"

class Flyer :public Enemy {
	AirFollowing* airFollow;
};

class Bomber :public Enemy {
	AirFollowing* airFollow;
	AirDash* airDash;
	//Explosion
	
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
};

class DeQueen :public Enemy {

};