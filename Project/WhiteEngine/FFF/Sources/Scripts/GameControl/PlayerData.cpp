#include "PlayerData.hpp"
#include "../../EquipmentManager.hpp"

#include "Core/Logger.hpp"

LeaderboardEntry::LeaderboardEntry() {

}

LeaderboardEntry::LeaderboardEntry(std::string name, int score) {
	Name = name;
	Score = score;
}

//initialize weapons/artifacts unlock data
PlayerData::PlayerData() {
	Weapons = vector<bool>(EquipmentManager::totalWeapon, false);
	Artifacts = vector<bool>(EquipmentManager::totalArtifact, false);

	Weapons[WEAPON_TYPE::WEAPON_MACHINEGUN] = true;
}

void PlayerData::AddLeaderboardEntry(std::string name, int score) {
	Leaderboard.insert(std::make_shared<LeaderboardEntry>(name, score));
}

void PlayerData::PrintLeaderboard() {
	ENGINE_INFO("========== LEADER BOARD ===========");
	int c = 0;
	for (std::shared_ptr<LeaderboardEntry> en : Leaderboard) {
		ENGINE_INFO("#{}: {}     {}", c, en->Name, en->Score);
		++c;
	}
}