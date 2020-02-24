#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/TextRenderer.hpp"
#include "Utility/ObjectPool.h"
#include "HPsystem.hpp"
#include "EnemySpawner.hpp"

#include <map>

enum POOL_TYPE {
	BULLET_MG = 0,
	ENEMY_FLYER,
	ENEMY_BOMBER
};

class GameController : public BehaviourScript {
private:
	static GameController* instance;
	float ScoreValue = 0;
	float ComboValue = 1;

	float startHPscaleX;
	float startHPscaleY;

	float startHPposX;
	
	HPsystem* PlayerHP;
	GameObject* HPbar;
	GameObject* ScoreText;

	map<int, ObjectPool*> Pools;
	GameObject* FlyerSpawner;
	GameObject* BomberSpawner;

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

	void AssignScoreText(GameObject* ScoreText);
	void AssignHPbar(GameObject* hpbar);
	void AssignPlayer(GameObject* player);

	void AddPool(ObjectPool* pool, int type);
	ObjectPool* GetPool(int type);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};