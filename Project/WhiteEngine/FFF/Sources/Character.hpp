#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Physic/PhysicScene.hpp"
#include "Input/Input.hpp"
#include "Core/EC/GameObject.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class Character : public BehaviourScript {
public:
	bool facingRight = true;
	void flip();

	virtual void OnAwake() override {}
	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			facingRight
			);
	}
};

CEREAL_REGISTER_TYPE(Character);