#pragma once
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"

class Equipment 
{
protected:
	GameObject* modifyObject;
public:
	virtual void Modify(GameObject* obj) = 0;
	virtual void GameTimeBehaviour(float dt) = 0;
	void SetmodifyObject(GameObject* obj) { this->modifyObject = obj; }
	GameObject* GetmodifyObject(){ return modifyObject; }
};
