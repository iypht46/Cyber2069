#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "EnemySpawner.hpp"
#include "PlayerController.hpp"

#include "Weapon.hpp"
#include "Artifact.hpp"

#include <memory>
#include <map>

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

enum ITEM_TYPE {
	ALL = 0,
	WEAPON,
	ARTIFACT
};

class EquipmentManager : public BehaviourScript {
private:
	std::shared_ptr<GameObject> playerObj;

	int totalWeapon = 5;
	int totalArtifact = 7;

	int maxPlayerWeapon = 1;
	int maxPlayerArtifact = 2;

	vector<std::shared_ptr<GameObject>> m_weaponObjs;
	vector<std::shared_ptr<Artifact>> m_artifacts;
	
	vector<bool> Unlock_Weapons;
	vector<bool> Unlock_Artifacts;

	int Weapon_Buffer = -1;
	int* Artifact_Buffer;

	int weaponCount = 0;
	int artifactCount = 0;
public:

	EquipmentManager();
	void OnAwake();

	void AssignPlayer(std::shared_ptr<GameObject> player) { this->playerObj = player; }
	void AssignWeaponToManager(std::shared_ptr<GameObject> weaponObj);
	void AssignArtifactToManager(std::shared_ptr<Artifact> artifact);

	int GetMaxWeapon() { return maxPlayerWeapon; }
	int GetMaxArtifact() { return maxPlayerArtifact; }
	int GetTotalWeapon() { return totalWeapon; }
	int GetTotalArtifact() { return totalArtifact; }


	//===============Unlock System===============

	//reset the vector to false bool with size of total weapon/artifact
	void InitData();

	//Random Unlock weapon & artifact
	void Unlock_RandomAll();
	bool Unlock_RandomWeapon();
	bool Unlock_RandomArtifact();

	//Unlock Specific Type
	void Unlock_WEAPON(int type);
	void Unlock_ARTIFACT(int type);

	//Check all
	bool isAllWeaponUnlock();
	bool isAllArtifactUnlock();
	bool isAllUnlock();
	//Check Specific Type
	bool isWeaponUnlock(int type);
	bool isArtifactUnlock(int type);
	
	//Set and get the unlock vector
	vector<bool> GetWeaponUnlockData() { return Unlock_Weapons; }
	vector<bool> GetArtifactUnlockData() { return Unlock_Artifacts; }
	void SetWeaponUnlockData(vector<bool> data) {this->Unlock_Weapons = data; }
	void SetArtifactUnlockData(vector<bool> data) { this->Unlock_Artifacts = data; }



	//===============Add/Remove===============
	vector<std::shared_ptr<GameObject>> GetAllWeaponObjs() { return m_weaponObjs; }
	vector<std::shared_ptr<Artifact>> GetAllArtifact() { return m_artifacts; }

	std::shared_ptr<GameObject> GetWeaponObjs(int type);
	std::shared_ptr<Artifact> GetArtifact(int type);

	//Return true if success, false if fail, duplicate, lock or Player's equipment is full
	bool AddPlayerWeapon(int type);
	bool AddPlayerArtifact(int type);

	bool RemovePlayerWeapon(int type);
	bool RemovePlayerArtifact(int type);

	void SetWeaponBuffer(int type);
	void SetArtifactBuffer(int index, int type) { Artifact_Buffer[index] = type; }
	
	void AddArtifactBuffer(int type);

	int GetWeaponBuffer() { return Weapon_Buffer; }
	int* GetArtifactBuffer() { return Artifact_Buffer; }

	void InitPlayerEquipment();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			playerObj,
			totalWeapon,
			totalArtifact,
			maxPlayerWeapon,
			maxPlayerArtifact,
			m_weaponObjs,
			m_artifacts
		);
	}
};

CEREAL_REGISTER_TYPE(EquipmentManager);
