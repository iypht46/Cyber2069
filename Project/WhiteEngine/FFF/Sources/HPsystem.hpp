#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"

#include <cereal/types/polymorphic.hpp>

class HPsystem : public BehaviourScript {
protected:
	float Maxhp;
	bool invincible = false;

	float hp = 1;
	bool dead = false;
public:
	void SetMaxHP(float hp);
	void SetHp(float hp);
	void SetInvincible(bool inv);

	float GetMaxHP();
	float GetHP();

	void ResetHP(); // HP = maxHP , dead = false

	void TakeDamage(float damage);
	bool isDead();

	void Dead();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();

	//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			Maxhp,
			invincible
		);
	}
};

CEREAL_REGISTER_TYPE(HPsystem);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BehaviourScript, HPsystem);