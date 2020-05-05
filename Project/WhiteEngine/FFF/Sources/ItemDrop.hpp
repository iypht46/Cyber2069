#pragma once

#include "Core/EC/Components/BehaviourScript.h"

#include "GameController.hpp"
#include "EquipmentManager.hpp"
#include "PlayerController.hpp"


#include "Graphic/Camera.hpp"

class ItemDrop : public BehaviourScript 
{
private:
	Graphic::CameraObject* cam;
	Rigidbody* rb;

	EquipmentManager* eqManager = nullptr;
	PlayerController* player = nullptr;

	pair<int, int> itemtype;

	unsigned int defaultTex;
public:
	virtual void OnAwake(); 
	virtual void OnStart();
	virtual void OnEnable();
	virtual void OnUpdate(float dt);
	virtual void OnTriggerEnter(const Physic::Collision);

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this)
		);
	}
};

CEREAL_REGISTER_TYPE(ItemDrop);
