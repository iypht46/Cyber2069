#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/UIComponents/TextRenderer.hpp"

#include "Graphic/GLRenderer.h"

#include "GameController.hpp"
#include "EquipmentManager.hpp"

#include <vector>

class LoadoutUI : public BehaviourScript 
{
private:
	GameController* gameControl = nullptr;
	EquipmentManager* eqManager = nullptr;
	
	std::shared_ptr<GameObject> WeaponDisplaySlot;
	vector<std::shared_ptr<GameObject>> ArtifactDisplaySlot;

	vector<std::shared_ptr<GameObject>> WeaponSelectButtons;
	vector<std::shared_ptr<GameObject>> ArtifactSelectButtons;

	string eqDescription = "";

	unsigned int defaultWeaponDisplayTex;
	unsigned int defaultArtfDisplayTex;
public:
	LoadoutUI();

	void StartGameplay();

	void AssignSelectButton(std::shared_ptr<GameObject> selectButton);

	void AssignWeaponDisplaySlot(std::shared_ptr<GameObject> WeaponDisplaySlot);
	void AssignArtifactDisplaySlot(std::shared_ptr<GameObject> ArtifactDisplaySlot);

	void UpdateDisplaySlotTexture();

	virtual void OnAwake();

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnUpdate(float dt);

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			WeaponDisplaySlot,
			ArtifactDisplaySlot,
			WeaponSelectButtons,
			ArtifactSelectButtons
		);
	}
};

CEREAL_REGISTER_TYPE(LoadoutUI);

class LoadoutSelectButton : public BehaviourScript 
{
private:
	LoadoutUI* loadoutUI = nullptr;
	EquipmentManager* eqManager = nullptr;

public:
	int type = 0;
	int equipment_type = 0;
	string description = "";

	void SetLoadoutUI(LoadoutUI* ui) { this->loadoutUI = ui; }
	void SetEquipmentManager(EquipmentManager* eq) { this->eqManager = eq; }
	void SetType(int type) { this->type = type; }
	void SetEquipmentType(int type) { this->equipment_type = type; };

	void AddEquipment();

	void RemoveWeapon();
	void RemoveArtifact(int index);

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			type,
			equipment_type,
			description
		);
	}
};

CEREAL_REGISTER_TYPE(LoadoutSelectButton);