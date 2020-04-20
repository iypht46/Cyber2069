#pragma once

#include "Core/EC/Components/Transform.hpp"
#include "Utility/ObjectPool.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class GameObject;

class ObjectShooter : public BehaviourScript {
public:
	float bullet_speed = 300.0f;
	float firerate = 2.0f;

	Transform* bulletSpawnPos = nullptr;

	ObjectPool* BulletPool;

	void Shoot(Transform* target);

	virtual void OnAwake() override;
	virtual void OnFixedUpdate(float dt) override;

	ObjectShooter();
private:
	float weapon_delay_count;

public:
	int sr_BulletObjectTypeAsInt = 0;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			bullet_speed,
			firerate,
			sr_BulletObjectTypeAsInt
			);
	}
};