#pragma once
#include <memory>

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/TextRenderer.hpp"
#include "HPsystem.hpp"

class GameController : public BehaviourScript {
private:
	static GameController* instance;
	float ScoreValue = 0;
	float ComboValue = 1;

	float startHPscaleX;
	float startHPscaleY;

	float startHPposX;
	
	std::shared_ptr<HPsystem>  PlayerHP;
	std::shared_ptr<GameObject> HPbar;
	std::shared_ptr<GameObject> ScoreText;
public:
	GameController();
	static GameController* GetInstance();
	float GetScore();
	float GetCombo();

	void SetScore(float score);

	void AddScoreValue(float baseScore);
	void AddComboValue(float value);

	void ResetScore();

	void updateHPui();

	void AssignScoreText(std::shared_ptr<GameObject> ScoreText);
	void AssignHPbar(std::shared_ptr<GameObject> hpbar);
	void AssignPlayer(std::shared_ptr<GameObject> player);
	
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};