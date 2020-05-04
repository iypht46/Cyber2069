#include "EquipmentManager.hpp"


int EquipmentManager::totalWeapon = 5;
int EquipmentManager::totalArtifact = 7;

int EquipmentManager::maxPlayerWeapon = 1;
int EquipmentManager::maxPlayerArtifact = 2;

EquipmentManager::EquipmentManager(){}

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

	int randEqType = rand() % 2;

	switch (randEqType)
	{
	case 1:
		if (!Unlock_RandomWeapon()) 
		{
			Unlock_RandomArtifact();
		}
		break;
	case 2:
		if (!Unlock_RandomArtifact())
		{
			Unlock_RandomWeapon();
		}
		break;
	default:
		break;
	}

}

bool EquipmentManager::Unlock_RandomWeapon() 
{
	if (isAllWeaponUnlock()) 
	{
		return false;
	}

	int randType = rand() % totalWeapon;

	while (isWeaponUnlock(randType)) 
	{
		randType = rand() % totalWeapon;
	}

	Unlock_WEAPON(randType);

	return true;
}

bool EquipmentManager::Unlock_RandomArtifact()
{
	if (isAllArtifactUnlock())
	{
		return false;
	}

	int randType = rand() % totalArtifact;

	while (isArtifactUnlock(randType))
	{
		randType = rand() % totalArtifact;
	}

	Unlock_ARTIFACT(randType);

	return true;
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

	AddPlayerWeapon(Weapon_Buffer);
	
	for (int i = 0; i < maxPlayerArtifact; i++)
	{
		if (Artifact_Buffer[i] != -1) 
		{
			AddPlayerArtifact(Artifact_Buffer[i]);
		}
	}

	player->ModifyFromEquipment();
}
