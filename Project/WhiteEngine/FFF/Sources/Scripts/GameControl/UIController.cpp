#include "UIController.h"

#include <string>

#include "Core/EC/GameObject.hpp"
#include "Core/EC/UIComponents/TextRenderer.hpp"

#include "../../GameController.hpp"
#include "../../EquipmentManager.hpp"
#include "../../PlayerController.hpp"
#include "../../HPsystem.hpp"

#include "Core//Logger.hpp"

UIController* UIController::_instance = nullptr;

UIController::UIController() {
	_instance = this;
}

UIController* UIController::GetInstance() {
	if (_instance != nullptr) {
		return _instance;
	}
	else {
		ENGINE_ERROR("There is no instance of UIController");
	}
}

void UIController::ToggleUI(int openGroup) {
	//close other group
	for (auto group : UIGroups) {
		if (group.first != openGroup)
		{
			for (std::weak_ptr<GameObject> uiobj : group.second) {

				if (!uiobj.expired()) {
					uiobj.lock()->SetActive(false);
				}
				else {
					ENGINE_WARN("uiobj is expired");
				}
			}
		}
	}

	//open target group
	for (auto group : UIGroups) {
		if (group.first == openGroup)
		{
			for (std::weak_ptr<GameObject> uiobj : group.second) {

				if (!uiobj.expired()) {
					uiobj.lock()->SetActive(false);
				}
				else {
					ENGINE_WARN("uiobj is expired");
				}
			}
		}
	}
}

void UIController::AdjustMasterVolume(float diff) {
	GameController::GetInstance()->SetMasterVolume(GameController::GetInstance()->MasterVolume + diff);
}

void UIController::AdjustMusicVolume(float diff) {
	GameController::GetInstance()->SetMusicVolume(GameController::GetInstance()->MusicVolume + diff);
}

void UIController::AdjustSFXVolume(float diff) {
	GameController::GetInstance()->SetSFXVolume(GameController::GetInstance()->SFXVolume + diff);
}

void UIController::UpdateEquipmentDisplay() {
	EquipmentManager* em = EquipmentManager::GetInstance();
	if (em != nullptr) {
		for (int i = 0; i < EquipmentManager::maxPlayerWeapon; ++i) {
			if (i < EquippedWeaponDisplay.size() && !EquippedWeaponDisplay[i].expired()) {
				EquippedWeaponDisplay[i].lock()->GetComponent<MeshRenderer>()->SetTexture(em->weaponItemTex[em->Weapon_Buffer]);
			}
		}

		for (int i = 0; i < EquipmentManager::maxPlayerArtifact; ++i) {
			if (i < EquippedArtifactDisplay.size() && !EquippedArtifactDisplay[i].expired()) {
				EquippedArtifactDisplay[i].lock()->GetComponent<MeshRenderer>()->SetTexture(em->weaponItemTex[em->Artifact_Buffer[i]]);
			}
		}
	}
}

void UIController::OnAwake() {
	this->PlayerHP = GameController::GetInstance()->PlayerHP;
	this->playerControl = GameController::GetInstance()->playerControl;

	startHPscaleX = HPbar.lock()->m_transform->GetScale().x;
	startHPscaleY = HPbar.lock()->m_transform->GetScale().y;
	startHPposX = HPbar.lock()->m_transform->GetPosition().x;

	startStaminascaleX = Staminabar.lock()->m_transform->GetScale().x;
	startStaminascaleY = Staminabar.lock()->m_transform->GetScale().y;
	startStaminaposX = Staminabar.lock()->m_transform->GetPosition().x;

	comboTextOgScale = ComboText.lock()->m_transform->GetScale();
}

void UIController::updateHPUI() {
	float playerHP = PlayerHP->GetHP();

	if (playerHP < 0)
	{
		playerHP = 0;
	}

	float currentX = (playerHP * startHPscaleX) / PlayerHP->GetMaxHP();
	float hpDiff = PlayerHP->GetMaxHP() - playerHP;

	float movePos = ((hpDiff / 2) * startHPscaleX) / PlayerHP->GetMaxHP();

	HPbar.lock()->m_transform->SetScale(glm::vec3(currentX, startHPscaleY, 1.0f));
	HPbar.lock()->m_transform->SetPosition(glm::vec3(startHPposX - movePos, HPbar.lock()->m_transform->GetPosition().y, 1.0f));

	HPText.lock()->GetComponent<TextRenderer>()->SetText(to_string((int)playerHP) + "/" + to_string((int)PlayerHP->GetMaxHP()));
	//HPText.lock()->m_transform->SetPosition(glm::vec3(startHPposX - movePos, HPbar.lock()->m_transform->GetPosition().y, 1.0f));
}

void UIController::updateStaminaUI()
{
	float playerSta = playerControl->GetStamina();

	if (playerSta < 0)
	{
		playerSta = 0;
	}

	float currentX = (playerSta * startStaminascaleX) / playerControl->GetMaxStamina();
	float staDiff = playerControl->GetMaxStamina() - playerSta;

	float movePos = ((staDiff / 2) * startStaminascaleX) / playerControl->GetMaxStamina();

	Staminabar.lock()->m_transform->SetScale(glm::vec3(currentX, startStaminascaleY, 1.0f));
	Staminabar.lock()->m_transform->SetPosition(glm::vec3(startStaminaposX - movePos, Staminabar.lock()->m_transform->GetPosition().y, 1.0f));
}

void UIController::updateScoreUI() {
	ScoreText.lock()->GetComponent<TextRenderer>()->SetText("Score: " + to_string((int)GameController::GetInstance()->ScoreValue));
	GameOverScoreText.lock()->GetComponent<TextRenderer>()->SetText("Score: " + to_string((int)GameController::GetInstance()->ScoreValue));

	float comboval = GameController::GetInstance()->ComboValue;
	float combosizeMultiplier = 5.0f * glm::log(((comboval + 69.0) / 70.0) + 1.0f);
	combosizeMultiplier = glm::clamp(combosizeMultiplier, 1.0f, 4.0f);

	ComboText.lock()->m_transform->SetScale(comboTextOgScale * combosizeMultiplier);
	ComboText.lock()->m_transform->SetRotation(10.0f * (combosizeMultiplier - 1.0f));
	ComboText.lock()->GetComponent<TextRenderer>()->SetText("x " + to_string((int)comboval));
}

void UIController::updateQueenHPUI() {
	float queenHP = QueenHP->GetHP();

	if (queenHP < 0)
	{
		queenHP = 0;
	}

	float currentX = (queenHP * startHPscaleX) / PlayerHP->GetMaxHP();
	//float hpDiff = QueenHP->GetMaxHP() - queenHP;

	QueenHPbar.lock()->m_transform->SetScale(glm::vec3(currentX, startHPscaleY, 1.0f));

	QueenHPText.lock()->GetComponent<TextRenderer>()->SetText(to_string((int)queenHP) + "/" + to_string((int)QueenHP->GetMaxHP()));
}

void UIController::UpdateVolumeTexts() {
	int maxValue = 100;

	MasterVolumeText.lock()->GetComponent<TextRenderer>()->SetText(to_string((int)(GameController::GetInstance()->MasterVolume * maxValue)));
	MusicVolumeText.lock()->GetComponent<TextRenderer>()->SetText(to_string((int)(GameController::GetInstance()->MusicVolume * maxValue)));
	SFXVolumeText.lock()->GetComponent<TextRenderer>()->SetText(to_string((int)(GameController::GetInstance()->SFXVolume * maxValue)));
}