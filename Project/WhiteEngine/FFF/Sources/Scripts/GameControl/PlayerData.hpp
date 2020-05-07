#pragma once

#include <set>
#include <vector>
#include <string>
#include <memory>

#include "Core/Logger.hpp"

#include <cereal/types/set.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

class LeaderboardEntry {
public:
	std::string Name = "White";
	int Score = 0;

	LeaderboardEntry();
	LeaderboardEntry(std::string name, int score);

//serialization
public:
	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			Name,
			Score
			);
	}
};

struct LeaderboardCompare {
	bool operator() (const std::shared_ptr<LeaderboardEntry> _l, const std::shared_ptr<LeaderboardEntry>_r) const {
		return _l->Score > _r->Score;
	}
};

class PlayerData {
public:
	std::vector<bool> Weapons;
	std::vector<bool> Artifacts;
	std::multiset<std::shared_ptr<LeaderboardEntry>, LeaderboardCompare> Leaderboard;

	PlayerData();

	void AddLeaderboardEntry(std::string name, int score); /*{
		std::shared_ptr<LeaderboardEntry> en = std::make_shared<LeaderboardEntry>(name, score);
		Leaderboard.insert(en);
	}*/
	void PrintLeaderboard(); /*{
		ENGINE_INFO("========== LEADER BOARD ===========");
		int c = 0;
		for (std::shared_ptr<LeaderboardEntry> en : Leaderboard) {
			ENGINE_INFO("#{}: {}     {}", c, en->Name, en->Score);
			++c;
		}
	}*/

	//serialization
public:
	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			Weapons,
			Artifacts,
			Leaderboard
			);
	}
};