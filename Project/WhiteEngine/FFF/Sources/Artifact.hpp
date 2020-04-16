#pragma once
#include "Equipment.hpp"

class Artifact : public Equipment 
{
public:
	virtual void Modify() = 0;
	virtual void Revert() = 0;
	virtual void GameTimeBehaviour(float dt) = 0;
};

class BulletAmplifier : public Artifact 
{
private:
	float multiplier_weapon = 2.0f;
	float multiplier_speedDecrease = 0.75f;
public:
	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class FireRateUP : public Artifact 
{
private:
	float multiplier_firerate = 2.0f;
public:
	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class AttackUP : public Artifact 
{
private:
	float multiplier_attack = 2.0f;
public:
	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class LowGravity : public Artifact 
{
private:
	float multiplier_GravityScale = 2.0f;
public:
	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class SpeedRunner : public Artifact {
private:
	float multiplier_moveSpeed = 2.0f;
public:
	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class ArtifactAmplifier : public Artifact 
{
public:
	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class CursedPendant : public Artifact {
public:
	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};