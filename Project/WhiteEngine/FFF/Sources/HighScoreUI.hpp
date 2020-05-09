#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/UIComponents/TextRenderer.hpp"

#include "GameController.hpp"
#include "EquipmentManager.hpp"
#include "Scripts/GameControl/PlayerData.hpp"

class HighScoreUI : public BehaviourScript {
private:
	TextRenderer* nametext;
	TextRenderer* scoretext;
	std::multiset<std::shared_ptr<LeaderboardEntry>, LeaderboardCompare> ldb;

	string s_score = "";
	string s_name = "";

	int maxEntryShow = 10;
public:
	std::shared_ptr<GameObject> NameText;
	std::shared_ptr<GameObject> ScoreText;

	void UpdateScoreBoard();

	virtual void OnAwake();

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnUpdate(float dt);

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this)
		);
	}
};

CEREAL_REGISTER_TYPE(HighScoreUI);