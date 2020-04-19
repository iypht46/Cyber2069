#pragma once
#include "Equipment.hpp"

enum ARTIFACT_TYPE {
	ARTF_BULLETAMP = 0,
	ARTF_FIRERATEUP,
	ARTF_SPEEDRUNNER,
	ARTF_ATKUP,
	ARTF_LOWGRAV,
	ARTF_ARTIFACTAMP,
	ARTF_CURSEDPENDANT
};

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
	BulletAmplifier() { type = ARTIFACT_TYPE::ARTF_BULLETAMP; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class FireRateUP : public Artifact 
{
private:
	float multiplier_firerate = 2.0f;
public:
	FireRateUP() { type = ARTIFACT_TYPE::ARTF_FIRERATEUP; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class AttackUP : public Artifact 
{
private:
	float multiplier_attack = 2.0f;
public:
	AttackUP() { type = ARTIFACT_TYPE::ARTF_ATKUP; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class LowGravity : public Artifact 
{
private:
	float multiplier_GravityScale = 2.0f;
public:
	LowGravity() { type = ARTIFACT_TYPE::ARTF_LOWGRAV; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class SpeedRunner : public Artifact {
private:
	float multiplier_moveSpeed = 2.0f;
public:
	SpeedRunner() { type = ARTIFACT_TYPE::ARTF_SPEEDRUNNER; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class ArtifactAmplifier : public Artifact 
{
public:
	ArtifactAmplifier() { type = ARTIFACT_TYPE::ARTF_ARTIFACTAMP; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};

class CursedPendant : public Artifact {
public:
	CursedPendant() { type = ARTIFACT_TYPE::ARTF_CURSEDPENDANT; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}
};