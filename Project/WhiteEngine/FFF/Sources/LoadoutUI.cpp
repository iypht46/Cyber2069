#include "LoadoutUI.hpp"
#include "Input/Input.hpp"


LoadoutUI::LoadoutUI() 
{
	gameControl = GameController::GetInstance();
	eqManager = gameControl->GetGameObject()->GetComponent<EquipmentManager>();

	WeaponDisplaySlot = nullptr;

	for (int i = 0; i < eqManager->GetTotalWeapon(); i++)
	{
		WeaponSelectButtons.push_back(nullptr);
	}

	for (int i = 0; i < eqManager->GetTotalArtifact(); i++)
	{
		ArtifactSelectButtons.push_back(nullptr);
	}

	for (int i = 0; i < eqManager->GetMaxArtifact(); i++)
	{
		ArtifactDisplaySlot.push_back(nullptr);
	}
}

void LoadoutUI::OnAwake() 
{
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
}

void LoadoutUI::OnUpdate(float dt) 
{
	if (Input::GetKeyDown(Input::KeyCode::KEY_1))
	{
		ArtifactSelectButtons[0]->GetComponent<LoadoutSelectButton>()->AddEquipment();
	}
	else if (Input::GetKeyDown(Input::KeyCode::KEY_2))
	{
		ArtifactSelectButtons[1]->GetComponent<LoadoutSelectButton>()->AddEquipment();
	}
	else if (Input::GetKeyDown(Input::KeyCode::KEY_3))
	{
		ArtifactSelectButtons[2]->GetComponent<LoadoutSelectButton>()->AddEquipment();
	}
	else if (Input::GetKeyDown(Input::KeyCode::KEY_4))
	{
		WeaponSelectButtons[0]->GetComponent<LoadoutSelectButton>()->AddEquipment();
	}
	else if (Input::GetKeyDown(Input::KeyCode::KEY_Q))
	{
		ArtifactDisplaySlot[0]->GetComponent<LoadoutSelectButton>()->RemoveArtifact(0);
	}
	else if (Input::GetKeyDown(Input::KeyCode::KEY_W))
	{
		ArtifactDisplaySlot[1]->GetComponent<LoadoutSelectButton>()->RemoveArtifact(1);
	}
	else if (Input::GetKeyDown(Input::KeyCode::KEY_E))
	{
		WeaponDisplaySlot->GetComponent<LoadoutSelectButton>()->RemoveWeapon();
	}
	else if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE)) 
	{
		StartGameplay();
	}


	UpdateDisplaySlotTexture();
}

void LoadoutUI::StartGameplay() 
{
	if (eqManager->GetWeaponBuffer() != -1) 
	{
		eqManager->InitPlayerEquipment();
		gameControl->SetGameState(GAME_STATE::GAMEPLAY);
		gameControl->GetPlayer()->GetComponent<PlayerController>()->assignWeapon();
	}
}

void LoadoutUI::OnDisable()
{
	GAME_INFO("onDisable");

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
}

void LoadoutUI::AssignSelectButton(std::shared_ptr<GameObject> selectButton) 
{
	LoadoutSelectButton* lsb = selectButton->GetComponent<LoadoutSelectButton>();

	if (lsb != nullptr) 
	{
		lsb->SetLoadoutUI(this);
		lsb->SetEquipmentManager(eqManager);

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
		lsb->SetEquipmentManager(eqManager);

		this->WeaponDisplaySlot = WeaponDisplaySlot;

		defaultWeaponDisplayTex = WeaponDisplaySlot->GetComponent<MeshRenderer>()->GetTexture();
	}
}

void LoadoutUI::AssignArtifactDisplaySlot(std::shared_ptr<GameObject> ArtifactDisplaySlot) 
{
	LoadoutSelectButton* lsb = ArtifactDisplaySlot->GetComponent<LoadoutSelectButton>();

	if (lsb != nullptr)
	{
		lsb->SetLoadoutUI(this);
		lsb->SetEquipmentManager(eqManager);

		if (this->ArtifactDisplaySlot[0] == nullptr) {
			this->ArtifactDisplaySlot[0] = ArtifactDisplaySlot;
		}
		else {
			this->ArtifactDisplaySlot[1] = ArtifactDisplaySlot;
		}

		defaultArtfDisplayTex = ArtifactDisplaySlot->GetComponent<MeshRenderer>()->GetTexture();
	}
	
}

void LoadoutUI::UpdateDisplaySlotTexture() 
{
	if (eqManager->GetWeaponBuffer() != -1) 
	{
		unsigned int tex = WeaponSelectButtons[eqManager->GetWeaponBuffer()]->GetComponent<MeshRenderer>()->GetTexture();
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
			unsigned int tex = ArtifactSelectButtons[eqManager->GetArtifactBuffer()[i]]->GetComponent<MeshRenderer>()->GetTexture();

			ArtifactDisplaySlot[i]->GetComponent<MeshRenderer>()->SetTexture(tex);
		}
		else 
		{
			ArtifactDisplaySlot[i]->GetComponent<MeshRenderer>()->SetTexture(defaultArtfDisplayTex);
		}
	}
}

void LoadoutSelectButton::AddEquipment() 
{
	switch (type)
	{
	case ITEM_TYPE::WEAPON:
		eqManager->SetWeaponBuffer(equipment_type);
		break;
	case ITEM_TYPE::ARTIFACT:
		eqManager->AddArtifactBuffer(equipment_type);
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