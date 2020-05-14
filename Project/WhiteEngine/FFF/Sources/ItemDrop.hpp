#pragma once

#include "Core/EC/Components/BehaviourScript.h"

#include "GameController.hpp"
#include "EquipmentManager.hpp"
#include "PlayerController.hpp"
#include "Graphic/Texture.hpp"

#include "Graphic/Camera.hpp"

enum Drop_Type {
	Unlock = 0,
	Heal
};

class ItemDrop : public BehaviourScript 
{
private:
	Graphic::CameraObject* cam;
	Rigidbody* rb;

	EquipmentManager* eqManager = nullptr;
	PlayerController* player = nullptr;
	SoundPlayer* sp = nullptr;

	int dropType = 0;
	pair<int, int> itemtype;

	float HealValue;

	//unsigned int defaultTex;
	Graphic::Texture defaultTex;

public:
	void SetType(int dropType);
	void SetHealValue(float value) { this->HealValue = value; }

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
