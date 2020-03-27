#pragma once
#include <memory>

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/TextRenderer.hpp"
#include "HPsystem.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class GameController : public BehaviourScript {
private:
	static GameController* instance;
	float ScoreValue = 0;
	float ComboValue = 1;

	float startHPscaleX;
	float startHPscaleY;

	float startHPposX;
	
	HPsystem*  PlayerHP;
	GameObject* HPbar;
	GameObject* ScoreText;
public:
	GameController();
	~GameController() {}

	static GameController* GetInstance();
	float GetScore();
	float GetCombo();

	void SetScore(float score);

	void AddScoreValue(float baseScore);
	void AddComboValue(float value);

	void ResetScore();

	void updateHPui();

	void AssignScoreText(GameObject* ScoreText);
	void AssignHPbar(GameObject* hpbar);
	void AssignPlayer(GameObject* player);
	
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			ScoreValue,
			ComboValue,
			startHPscaleX,
			startHPscaleY,
			startHPposX
			);
	}
};

CEREAL_REGISTER_TYPE(GameController);