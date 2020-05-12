#pragma once
#include "Equipment.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

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
	bool isAmplify = false;

	virtual void Modify() = 0;
	virtual void Revert() = 0;
	virtual void GameTimeBehaviour(float dt) = 0;

//serialize
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Equipment>(this)
			);
	}
};

CEREAL_REGISTER_TYPE(Artifact);

class BulletAmplifier : public Artifact 
{
private:
	float multiplier_weapon = 2.0f;
	float multiplier_speedDecrease = 0.75f;

	float multiplier_amplifier = 2.0f;
public:
	BulletAmplifier() { type = ARTIFACT_TYPE::ARTF_BULLETAMP; }

	void Modify();
	void Revert();

	void GameTimeBehaviour(float dt) {}

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Artifact>(this),
			multiplier_weapon,
			multiplier_speedDecrease,
			multiplier_amplifier
			);
	}
};
CEREAL_REGISTER_TYPE(BulletAmplifier);

class FireRateUP : public Artifact 
{
private:
	float multiplier_firerate = 2.0f;
	float multiplier_amplifier = 2.0f;
public:
	FireRateUP() { type = ARTIFACT_TYPE::ARTF_FIRERATEUP; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Artifact>(this),
			multiplier_firerate,
			multiplier_amplifier
			);
	}
};
CEREAL_REGISTER_TYPE(FireRateUP);


class AttackUP : public Artifact 
{
private:
	float multiplier_attack = 2.0f;
	float multiplier_amplifier = 2.0f;
public:
	AttackUP() { type = ARTIFACT_TYPE::ARTF_ATKUP; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Artifact>(this),
			multiplier_attack,
			multiplier_amplifier
			);
	}
};
CEREAL_REGISTER_TYPE(AttackUP);


class LowGravity : public Artifact 
{
private:
	float multiplier_GravityScale = 2.0f;
	float multiplier_amplifier = 2.0f;
public:
	LowGravity() { type = ARTIFACT_TYPE::ARTF_LOWGRAV; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Artifact>(this),
			multiplier_GravityScale,
			multiplier_amplifier
			);
	}
};
CEREAL_REGISTER_TYPE(LowGravity);


class SpeedRunner : public Artifact {
private:
	float multiplier_moveSpeed = 2.0f;
	float multiplier_amplifier = 2.0f;
public:
	SpeedRunner() { type = ARTIFACT_TYPE::ARTF_SPEEDRUNNER; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Artifact>(this),
			multiplier_moveSpeed,
			multiplier_amplifier
			);
	}
};
CEREAL_REGISTER_TYPE(SpeedRunner);


class ArtifactAmplifier : public Artifact 
{
public:
	ArtifactAmplifier() { type = ARTIFACT_TYPE::ARTF_ARTIFACTAMP; }

	void Modify();
	void Revert();
	void GameTimeBehaviour(float dt) {}

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Artifact>(this)
			);
	}
};
CEREAL_REGISTER_TYPE(ArtifactAmplifier);


class CursedPendant : public Artifact {
public:
	CursedPendant() { type = ARTIFACT_TYPE::ARTF_CURSEDPENDANT; }

	void Modify();
	void Revert();

	void GameTimeBehaviour(float dt) {}

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Artifact>(this)
			);
	}
};
CEREAL_REGISTER_TYPE(CursedPendant);
