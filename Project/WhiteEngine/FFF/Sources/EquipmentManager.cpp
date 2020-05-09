#include "EquipmentManager.hpp"

int EquipmentManager::totalWeapon = 5;
int EquipmentManager::totalArtifact = 7;

int EquipmentManager::maxPlayerWeapon = 1;
int EquipmentManager::maxPlayerArtifact = 2;

EquipmentManager* EquipmentManager::_instance = nullptr;

EquipmentManager::EquipmentManager() {
	_instance = this;
}

EquipmentManager* EquipmentManager::GetInstance() {
	if (_instance != nullptr) {
		return _instance;
	}
	else {
		ENGINE_ERROR("There is no instance of Equipmentmanager");
	}
}

void EquipmentManager::OnAwake() 
{
	//Read from PlayerData

	//if dont have in player data
	InitData();

	Artifact_Buffer = new int[maxPlayerArtifact];

	for (int i = 0; i < maxPlayerArtifact; i++) 
	{
		Artifact_Buffer[i] = -1;
	}

	weaponItemTex = new int[totalWeapon];
	artifactItemTex = new int[totalArtifact];

	for (int i = 0; i < totalWeapon; i++)
	{
		weaponItemTex[i] = -1;
	}

	for (int i = 0; i < totalArtifact; i++)
	{
		artifactItemTex[i] = -1;
	}

}

void EquipmentManager::OnUpdate(float dt) 
{
}

void EquipmentManager::AssignWeaponToManager(std::shared_ptr<GameObject> weaponObj)
{
	m_weaponObjs.push_back(weaponObj);
}

void EquipmentManager::AssignArtifactToManager(std::shared_ptr<Artifact> artifact)
{
	m_artifacts.push_back(artifact);
}

void EquipmentManager::InitData() 
{
	Unlock_Weapons.clear();
	Unlock_Artifacts.clear();

	for (int i = 0; i < totalWeapon; i++) 
	{
		Unlock_Weapons.push_back(false);
	}

	for (int i = 0; i < totalArtifact; i++)
	{
		Unlock_Artifacts.push_back(false);
	}
}

void EquipmentManager::Unlock_RandomAll() 
{
	if (isAllUnlock()) 
	{
		return;
	}
	
	pair<int, int> item = GetRandomType();

	Unlock(item.first, item.second);
}

bool EquipmentManager::Unlock_RandomWeapon() 
{
	if (isAllWeaponUnlock()) 
	{
		return false;
	}

	Unlock_WEAPON(GetRandomType_Weapon());
	
	return true;
}

bool EquipmentManager::Unlock_RandomArtifact()
{
	if (isAllArtifactUnlock())
	{
		return false;
	}

	Unlock_ARTIFACT(GetRandomType_Artifact());
	
	return false;
}

pair<int, int> EquipmentManager::GetRandomType() 
{
	if (isAllUnlock())
	{
		return pair<int, int>(-1, -1);
	}

	int itemType = (rand() % 2) + 1;

	int type;

	switch (itemType)
	{
	case 1:
		type = GetRandomType_Weapon();

		if (type == -1)
		{
			itemType = 2;
			type = GetRandomType_Artifact();
		}
		break;
	case 2:
		type = GetRandomType_Artifact();

		if (type == -1)
		{
			itemType = 1;
			type = GetRandomType_Weapon();
		}
		break;
	default:
		break;
	}

	return pair<int, int>(itemType, type);
}

int EquipmentManager::GetRandomType_Artifact() {
	
	if (isAllArtifactUnlock())
	{
		return -1;
	}

	int randType = rand() % totalArtifact;

	while (isArtifactUnlock(randType))
	{
		randType = rand() % totalArtifact;
	}

	return randType;
}

int EquipmentManager::GetRandomType_Weapon() 
{
	if (isAllWeaponUnlock())
	{
		return -1;
	}

	int randType = rand() % totalWeapon;

	while (isWeaponUnlock(randType))
	{
		randType = rand() % totalWeapon;
	}

	return randType;
}

void EquipmentManager::Unlock(int itemtype, int type) 
{
	switch (itemtype)
	{
	case ITEM_TYPE::WEAPON:
		Unlock_WEAPON(type);
		break;
	case ITEM_TYPE::ARTIFACT:
		Unlock_ARTIFACT(type);
		break;
	default:
		break;
	}
}

void EquipmentManager::Unlock_WEAPON(int type) 
{
	Unlock_Weapons.at(type) = true;
}

void EquipmentManager::Unlock_ARTIFACT(int type)
{
	Unlock_Artifacts.at(type) = true;
}

bool EquipmentManager::isAllWeaponUnlock() 
{
	for (bool b : Unlock_Weapons) 
	{
		if (!b) 
		{
			return false;
		}
	}

	return true;
}

bool EquipmentManager::isAllArtifactUnlock()
{
	for (bool b : Unlock_Artifacts)
	{
		if (!b)
		{
			return false;
		}
	}

	return true;
}

bool EquipmentManager::isAllUnlock() 
{
	return (isAllArtifactUnlock() && isAllWeaponUnlock());
}

bool EquipmentManager::isWeaponUnlock(int type)
{
	if (type >= Unlock_Weapons.size()) 
	{
		return false;
	}

	return Unlock_Weapons.at(type);
}

bool EquipmentManager::isArtifactUnlock(int type) 
{
	if (type >= Unlock_Artifacts.size())
	{
		return false;
	}

	return Unlock_Artifacts.at(type);
}

std::shared_ptr<GameObject> EquipmentManager::GetWeaponObjs(int type)
{
	for (std::shared_ptr<GameObject> obj : m_weaponObjs) 
	{
		Weapon* w = obj->GetComponent<Weapon>();

		if (w != nullptr) 
		{

			if (w->GetType() == type) 
			{
				return obj;
			}
		}
	}

	return nullptr;
}

std::shared_ptr<Artifact> EquipmentManager::GetArtifact(int type)
{
	for (std::shared_ptr<Artifact> a : m_artifacts) 
	{
		if (a->GetType() == type) 
		{
			return a;
		}
	}

	return nullptr;
}

bool EquipmentManager::AddPlayerWeapon(int type)
{
	PlayerController* player = playerObj->GetComponent<PlayerController>();

	if (type == -1) {
		return false;
	}

	if (!isWeaponUnlock(type)) 
	{
		return false;
	}

	if (weaponCount == maxPlayerWeapon)
	{
		return false;
	}

	vector<Equipment*> equipments = player->GetEquipments();

	for (Equipment* e : equipments) 
	{
		if (Weapon* w = dynamic_cast<Weapon*>(e)) 
		{
			if (w->GetType() == type) 
			{
				return false;
			}
		}
	}

	std::shared_ptr<GameObject> obj = GetWeaponObjs(type);
	
	if (obj != nullptr) 
	{
		weaponCount++;
		player->AddEquipment(obj.get());
	}
	else {
		return false;
	}

	return true;
}

bool EquipmentManager::RemovePlayerWeapon(int type)
{
	PlayerController* player = playerObj->GetComponent<PlayerController>();

	if (weaponCount == 0) 
	{
		return false;
	}

	vector<Equipment*> equipments = player->GetEquipments();
	vector<Weapon*> weapons = player->GetWeapons();

	for (int i = 0; i < equipments.size(); i++) 
	{
		Equipment* e = equipments.at(i);

		if (Weapon* w = dynamic_cast<Weapon*>(e)) 
		{
			if (w->GetType() == type) 
			{
				player->RemoveEquipment(i);
			}
		}
	}

	for (int i = 0; i < weapons.size(); i++) 
	{
		Weapon* w = weapons.at(i);

		if (w->GetType() == type) 
		{
			player->RemoveWeapon(i);
			weaponCount--;

			return true;
		}
	}

	return false;
}

bool EquipmentManager::AddPlayerArtifact(int type)
{
	PlayerController* player = playerObj->GetComponent<PlayerController>();

	if (type == -1) {
		return false;
	}
	
	if (!isArtifactUnlock(type))
	{
		return false;
	}

	if (artifactCount == maxPlayerArtifact) 
	{
		return false;
	}

	vector<Equipment*> equipments = player->GetEquipments();

	for (Equipment* e : equipments)
	{
		if (Artifact* a = dynamic_cast<Artifact*>(e))
		{
			if (a->GetType() == type)
			{
				return false;
			}
		}
	}

	std::shared_ptr<Artifact> a = GetArtifact(type);

	if (a != nullptr) 
	{
		player->AddEquipment(a.get());
		artifactCount++;
	}

	return true;
}

bool EquipmentManager::RemovePlayerArtifact(int type)
{
	PlayerController* player = playerObj->GetComponent<PlayerController>();

	if (artifactCount == 0) 
	{
		return false;
	}

	vector<Equipment*> equipments = player->GetEquipments();

	for (int i = 0; i < equipments.size(); i++)
	{
		Equipment* e = equipments.at(i);

		if (Artifact* a = dynamic_cast<Artifact*>(e))
		{
			if (a->GetType() == type)
			{
				player->RemoveEquipment(i);
				artifactCount--;

				return true;
			}
		}
	}

	return false;
}

void EquipmentManager::SetWeaponBuffer(int type)
{
	Weapon_Buffer = type;
}

void EquipmentManager::AddArtifactBuffer(int type) 
{
	for (int i = 0; i < maxPlayerArtifact; i++) 
	{
		if (Artifact_Buffer[i] == type) 
		{
			return;
		}
	}

	if (Artifact_Buffer[0] == -1) 
	{
		Artifact_Buffer[0] = type;
	}
	else if (Artifact_Buffer[1] == -1) 
	{
		Artifact_Buffer[1] = type;
	}
	else {
		Artifact_Buffer[1] = Artifact_Buffer[0];
		Artifact_Buffer[0] = type;
	}
}

void EquipmentManager::InitPlayerEquipment() 
{
	PlayerController* player = playerObj->GetComponent<PlayerController>();

	if (Weapon_Buffer != -1) {
		AddPlayerWeapon(Weapon_Buffer);
		playerObj->GetComponent<PlayerController>()->assignWeapon();
	}
	
	for (int i = 0; i < maxPlayerArtifact; i++)
	{
		if (Artifact_Buffer[i] != -1) 
		{
			AddPlayerArtifact(Artifact_Buffer[i]);
		}
	}

	player->ModifyFromEquipment();
}

void EquipmentManager::ResetPlayerEquipment() 
{
	PlayerController* player = playerObj->GetComponent<PlayerController>();

	player->RevertEquipment();

	for (int i = 0; i < maxPlayerArtifact; i++)
	{
		Artifact_Buffer[i] = -1;
	}

	Weapon_Buffer = -1;

	artifactCount = 0;
	weaponCount = 0;
}
