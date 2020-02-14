#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/TextRenderer.hpp"

class GameController : public BehaviourScript {
private:
	static GameController* instance;
	float ScoreValue = 0;
	float ComboValue = 1;
	
	GameObject* ScoreText;
public:
	GameController();
	static GameController* GetInstance();
	float GetScore();
	float GetCombo();

	void SetScore(float score);

	void AddScoreValue(float baseScore);
	void AddComboValue(float value);

	void ResetScore();

	void AssignScoreText(GameObject* ScoreText);
	
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};