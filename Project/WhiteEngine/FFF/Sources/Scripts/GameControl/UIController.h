#pragma once
#include "Core/EC/Components/BehaviourScript.h"

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <memory>

#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

enum UI_GROUP {
	MainMenu = 0,
	Option,
	Credit,
	Loadout,		
	Gameplay,				//not include boss hp bar
	Pause,
	GameOver,
	Highscore,
	ResetProgressWarn,
	QuitGameWarn
};

//forward declaration
class GameObject;
class PlayerController;
class HPsystem;
class TextRenderer;

class UIController : public BehaviourScript{
public:
	static UIController* GetInstance();

	std::map<int, std::vector<std::weak_ptr<GameObject>>> UIGroups;		//all UI gameobject stored by group, use for turning on/off
	
	//Gameplay UI
	std::vector<std::weak_ptr<GameObject>> EquippedWeaponDisplay;
	std::vector<std::weak_ptr<GameObject>> EquippedArtifactDisplay;
	void UpdateEquipmentDisplay();
	
	//std::weak_ptr<GameObject> HPbarSlot_body;
	//std::weak_ptr<GameObject> HPbarSlot_tail;
	std::weak_ptr<GameObject> HPbar;
	std::weak_ptr<GameObject> HPText;
	//std::weak_ptr<GameObject> StaminabarSlot_body;
	//std::weak_ptr<GameObject> StaminabarSlot_tail;
	std::weak_ptr<GameObject> Staminabar;

	std::weak_ptr<GameObject> ScoreText;
	std::weak_ptr<GameObject> ComboText;

	std::weak_ptr<GameObject> QueenHPbar;
	std::weak_ptr<GameObject> QueenHPText;

	std::weak_ptr<GameObject> GameOverScoreText;

	std::weak_ptr<GameObject> MasterVolumeText;
	std::weak_ptr<GameObject> MusicVolumeText;
	std::weak_ptr<GameObject> SFXVolumeText;

	//SetActive the UI of openGroup true and turn other groups inactive
	void ToggleUI(int openGroup);
	void AdjustMasterVolume(float diff);
	void AdjustMusicVolume(float diff);
	void AdjustSFXVolume(float diff);

	UIController();

	virtual void OnAwake() override;

	void SetActiveQueenUI(bool active);

private:
	friend class GameController;
	static UIController* _instance;

	//gameplay ui temp var ----------------
	PlayerController* playerControl;
	HPsystem* PlayerHP;

	HPsystem* QueenHP;

	float startHPscaleX;
	float startHPscaleY;

	float startStaminascaleX;
	float startStaminascaleY;

	float startHPposX;
	float startStaminaposX;

	float startQueenHPscaleX;
	float startQueenHPscaleY;

	glm::vec3 comboTextOgScale;
	//--------------------------------------

	//gameplay update funct ----------------
	void updateHPUI();
	void updateStaminaUI();
	void updateScoreUI();

	void updateQueenHPUI();
	//--------------------------------------

	//menu update---------------------------
	void UpdateVolumeTexts();
	//--------------------------------------

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			cereal::defer(UIGroups)
			);
	}
};

CEREAL_REGISTER_TYPE(UIController);