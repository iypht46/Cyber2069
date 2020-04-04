#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Physic/PhysicScene.hpp"
#include "Input/Input.hpp"
#include "Core/EC/GameObject.hpp"

class Character : public BehaviourScript {
public:
	bool facingRight = true;
	void flip();
};