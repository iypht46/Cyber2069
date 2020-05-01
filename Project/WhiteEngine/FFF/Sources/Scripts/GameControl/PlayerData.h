#pragma once

#include <set>
#include <vector>
#include <string>
#include <memory>

#include "../../EquipmentManager.hpp"

#include <cereal/types/set.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

struct LeaderboardEntry {
	std::string Name = "White";
	int Score = 0;

	LeaderboardEntry(std::string name, int score) : Name(name), Score(score) {}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			Name,
			Score
			);
	}
};

struct LeaderboardCompare {
	bool operator() (const std::shared_ptr<LeaderboardEntry> _l, const std::shared_ptr<LeaderboardEntry>_r) {
		return _l->Score > _r->Score;
	}
};

struct PlayerData {
	UnlockData Weapons;
	UnlockData Artifacts;
	std::multiset<std::shared_ptr<LeaderboardEntry>, LeaderboardCompare> LeaderBoard;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			Weapons,
			Artifacts,
			LeaderBoard
			);
	}
};