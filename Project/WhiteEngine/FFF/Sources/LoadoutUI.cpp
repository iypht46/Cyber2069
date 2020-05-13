#include "LoadoutUI.hpp"
#include "Input/Input.hpp"


LoadoutUI::LoadoutUI() 
{
	WeaponDisplaySlot = nullptr;

	for (int i = 0; i < EquipmentManager::totalWeapon; i++)
	{
		WeaponSelectButtons.push_back(nullptr);
	}

	for (int i = 0; i < EquipmentManager::totalArtifact; i++)
	{
		ArtifactSelectButtons.push_back(nullptr);
	}

	for (int i = 0; i < EquipmentManager::maxPlayerArtifact; i++)
	{
		ArtifactDisplaySlot.push_back(nullptr);
	}
}

void LoadoutUI::OnAwake() 
{
	gameControl = GameController::GetInstance();
	eqManager = gameControl->GetGameObject()->GetComponent<EquipmentManager>();

	for (int i = 0; i < EquipmentManager::totalWeapon; i++)
	{
		if (WeaponSelectButtons[i] == nullptr) {
			continue;
		}

		LoadoutSelectButton* lsb = WeaponSelectButtons[i]->GetComponent<LoadoutSelectButton>();

		if (lsb != nullptr) 
		{
			lsb->SetEquipmentManager(eqManager);
		}
	}

	for (int i = 0; i < EquipmentManager::totalArtifact; i++)
	{
		if (ArtifactSelectButtons[i] == nullptr) {
			continue;
		}

		LoadoutSelectButton* lsb = ArtifactSelectButtons[i]->GetComponent<LoadoutSelectButton>();

		if (lsb != nullptr)
		{
			lsb->SetEquipmentManager(eqManager);
		}
	}

	for (int i = 0; i < EquipmentManager::maxPlayerArtifact; i++)
	{
		if (ArtifactDisplaySlot[i] == nullptr) {
			continue;
		}

		LoadoutSelectButton* lsb = ArtifactDisplaySlot[i]->GetComponent<LoadoutSelectButton>();

		if (lsb != nullptr)
		{
			lsb->SetEquipmentManager(eqManager);
		}
	}

	WeaponDisplaySlot->GetComponent<LoadoutSelectButton>()->SetEquipmentManager(eqManager);
}

void LoadoutUI::OnStart() {
	defaultWeaponDisplayTex = WeaponDisplaySlot->GetComponent<MeshRenderer>()->GetTextureObj();
	defaultArtfDisplayTex = ArtifactDisplaySlot[0]->GetComponent<MeshRenderer>()->GetTextureObj();

	for (int i = 0; i < EquipmentManager::totalWeapon; i++) 
	{
		if (WeaponSelectButtons[i] != nullptr) 
		{
			eqManager->weaponItemTex[i] = WeaponSelectButtons[i]->GetComponent<MeshRenderer>()->GetTextureObj();
		}
	}

	for (int i = 0; i < EquipmentManager::totalArtifact; i++)
	{
		if (ArtifactSelectButtons[i] != nullptr)
		{
			eqManager->artifactItemTex[i] = ArtifactSelectButtons[i]->GetComponent<MeshRenderer>()->GetTextureObj();
		}
	}

}

void LoadoutUI::OnEnable() 
{
	if (WeaponDisplaySlot != nullptr)
	{
		WeaponDisplaySlot->SetActive(true);
	}
	
	for (int i = 0; i < eqManager->GetMaxArtifact(); i++)
	{
		if(ArtifactDisplaySlot[i] != nullptr) {

			ArtifactDisplaySlot[i]->SetActive(true);
		}
	}

	for (int i = 0; i < eqManager->GetTotalWeapon(); i++)
	{
		if (eqManager->isWeaponUnlock(i) && (WeaponSelectButtons[i] != nullptr))
		{
			WeaponSelectButtons[i]->SetActive(true);
		}
	}

	for (int i = 0; i < eqManager->GetTotalArtifact(); i++)
	{
		if (eqManager->isArtifactUnlock(i) && (ArtifactSelectButtons[i] != nullptr))
		{
			ArtifactSelectButtons[i]->SetActive(true);
		}
	}

	eqNameObj->SetActive(true);
	eqDescriptionObj->SetActive(true);
}

void LoadoutUI::OnUpdate(float dt) 
{
	UpdateDisplaySlotTexture();
	UpdateDescriptionText();
}

void LoadoutUI::OnDisable()
{
	if (WeaponDisplaySlot != nullptr)
	{
		WeaponDisplaySlot->SetActive(false);
	}

	for (int i = 0; i < eqManager->GetMaxArtifact(); i++)
	{
		if (ArtifactDisplaySlot[i] != nullptr) 
		{
			ArtifactDisplaySlot[i]->SetActive(false);
		}
	}

	for (int i = 0; i < eqManager->GetTotalWeapon(); i++)
	{
		if (WeaponSelectButtons[i] != nullptr) 
		{
			WeaponSelectButtons[i]->SetActive(false);
		}
	}

	for (int i = 0; i < eqManager->GetTotalArtifact(); i++)
	{
		if (ArtifactSelectButtons[i] != nullptr)
		{
			ArtifactSelectButtons[i]->SetActive(false);
		}
	}

	eqNameObj->SetActive(false);
	eqDescriptionObj->SetActive(false);
}

void LoadoutUI::AssignSelectButton(std::shared_ptr<GameObject> selectButton) 
{
	LoadoutSelectButton* lsb = selectButton->GetComponent<LoadoutSelectButton>();

	if (lsb != nullptr) 
	{
		lsb->SetLoadoutUI(this);
		//lsb->SetEquipmentManager(eqManager);
		lsb->OnclickType = LOADOUTONCLICK_TYPE::ADD;

		switch (lsb->type)
		{
		case ITEM_TYPE::WEAPON:
			WeaponSelectButtons[lsb->equipment_type] = selectButton;
			break;
		case ITEM_TYPE::ARTIFACT:
			ArtifactSelectButtons[lsb->equipment_type] = selectButton;
			break;
		default:
			break;
		}
	}
}


void LoadoutUI::AssignWeaponDisplaySlot(std::shared_ptr<GameObject> WeaponDisplaySlot)
{
	LoadoutSelectButton* lsb = WeaponDisplaySlot->GetComponent<LoadoutSelectButton>();
	if (lsb != nullptr)
	{
		lsb->SetLoadoutUI(this);
		//lsb->SetEquipmentManager(eqManager);
		lsb->type = ITEM_TYPE::WEAPON;		
		lsb->OnclickType = LOADOUTONCLICK_TYPE::REMOVE;
		this->WeaponDisplaySlot = WeaponDisplaySlot;
	}
}

void LoadoutUI::AssignArtifactDisplaySlot(std::shared_ptr<GameObject> ArtifactDisplaySlot) 
{
	LoadoutSelectButton* lsb = ArtifactDisplaySlot->GetComponent<LoadoutSelectButton>();

	if (lsb != nullptr)
	{
		lsb->SetLoadoutUI(this);
		//lsb->SetEquipmentManager(eqManager);
		lsb->type = ITEM_TYPE::ARTIFACT;
		lsb->OnclickType = LOADOUTONCLICK_TYPE::REMOVE;
		//lsb->SetEquipmentManager(eqManager);

		if (this->ArtifactDisplaySlot[0] == nullptr) {
			this->ArtifactDisplaySlot[0] = ArtifactDisplaySlot;
			lsb->currIndex = 0;
		}
		else {
			this->ArtifactDisplaySlot[1] = ArtifactDisplaySlot;
			lsb->currIndex = 1;
		}
	}
	
}

void LoadoutUI::UpdateDisplaySlotTexture() 
{
	if (eqManager->GetWeaponBuffer() != -1) 
	{
		auto tex = WeaponSelectButtons[eqManager->GetWeaponBuffer()]->GetComponent<MeshRenderer>()->GetTextureObj();
		WeaponDisplaySlot->GetComponent<MeshRenderer>()->SetTexture(tex);
	}
	else 
	{
		WeaponDisplaySlot->GetComponent<MeshRenderer>()->SetTexture(defaultWeaponDisplayTex);
	}


	for (int i = 0; i < eqManager->GetMaxArtifact(); i++) 
	{
		if (eqManager->GetArtifactBuffer()[i] != -1) 
		{
			auto tex = ArtifactSelectButtons[eqManager->GetArtifactBuffer()[i]]->GetComponent<MeshRenderer>()->GetTextureObj();

			ArtifactDisplaySlot[i]->GetComponent<MeshRenderer>()->SetTexture(tex);
		}
		else 
		{
			ArtifactDisplaySlot[i]->GetComponent<MeshRenderer>()->SetTexture(defaultArtfDisplayTex);
		}
	}
}

void LoadoutUI::UpdateDescriptionText() 
{
	eqName = "";
	eqDescription = "";

	for (std::shared_ptr<GameObject> obj : ArtifactSelectButtons) 
	{
		if (obj == nullptr) {
			continue;
		}

		Button* bt = obj->GetComponent<Button>();

		if (bt == nullptr) {
			continue;
		}

		if (!bt->isOnHover) {
			continue;
		}

		LoadoutSelectButton* lsb = obj->GetComponent<LoadoutSelectButton>();

		if (lsb != nullptr) 
		{
			eqName = lsb->eq_name;
			eqDescription = lsb->eq_description;
		}
	}

	for (std::shared_ptr<GameObject> obj : WeaponSelectButtons)
	{
		if (obj == nullptr) {
			continue;
		}

		Button* bt = obj->GetComponent<Button>();

		if (bt == nullptr) {
			continue;
		}

		if (!bt->isOnHover) {
			continue;
		}

		LoadoutSelectButton* lsb = obj->GetComponent<LoadoutSelectButton>();

		if (lsb != nullptr)
		{
			eqName = lsb->eq_name;
			eqDescription = lsb->eq_description;
		}
	}
	
	eqNameObj->GetComponent<TextRenderer>()->SetText(eqName);
	eqDescriptionObj->GetComponent<TextRenderer>()->SetText(eqDescription);
}

void LoadoutSelectButton::OnUpdate(float dt) 
{
}

void LoadoutSelectButton::AddEquipment() 
{
	switch (type)
	{
	case ITEM_TYPE::WEAPON:
		if (eqManager->isWeaponUnlock(equipment_type)) 
		{
			eqManager->SetWeaponBuffer(equipment_type);
		}
		break;
	case ITEM_TYPE::ARTIFACT:
		if (eqManager->isArtifactUnlock(equipment_type))
		{
			eqManager->AddArtifactBuffer(equipment_type);
		}
		break;
	default:
		break;
	}
}

void LoadoutSelectButton::RemoveWeapon() 
{
	eqManager->SetWeaponBuffer(-1);
}

void LoadoutSelectButton::RemoveArtifact(int index)
{
	eqManager->SetArtifactBuffer(index, -1);
}