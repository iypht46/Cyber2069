#pragma once
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/GameObject.hpp"
#include "Utility/ObjectPool.h"

#include "Physic/PhysicScene.hpp"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"
#include "Enemy.hpp"

#include <cereal/types/polymorphic.hpp>

class Equipment
{
protected:
	int type = -1;
	GameObject* modifyObject;
public:
	virtual void Modify() = 0;
	virtual void GameTimeBehaviour(float dt) = 0;
	void SetmodifyObject(GameObject* obj) { this->modifyObject = obj; }
	GameObject* GetmodifyObject(){ return modifyObject; }
	int GetType() { return type; }

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			type
			);
	}
};
