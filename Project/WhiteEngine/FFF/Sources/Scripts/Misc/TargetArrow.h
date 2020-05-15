#pragma once

#include "Core/EC/Components/BehaviourScript.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class Transform;

class TargetArrow : public BehaviourScript {
private:
	bool inverse = false;

public:
	Transform* orbit = nullptr;
	Transform* target = nullptr;

	float distance = 100;

	virtual void OnAwake() override;
	virtual void OnFixedUpdate(float) override;

public:
	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			distance
			);
	}
};

CEREAL_REGISTER_TYPE(TargetArrow);
