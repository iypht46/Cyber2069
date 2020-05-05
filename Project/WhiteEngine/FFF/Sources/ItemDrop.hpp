#pragma once

#include "Core/EC/Components/BehaviourScript.h"

#include "GameController.hpp"
#include "EquipmentManager.hpp"
#include "PlayerController.hpp"

class ItemDrop : public BehaviourScript 
{
private:
	EquipmentManager* eqManager = nullptr;
	PlayerController* player = nullptr;

	pair<int, int> itemtype;
public:
	virtual void OnStart();
	virtual void OnEnable();
	virtual void OnTriggerEnter(const Physic::Collision) override;

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this)
		);
	}
};

CEREAL_REGISTER_TYPE(ItemDrop);
