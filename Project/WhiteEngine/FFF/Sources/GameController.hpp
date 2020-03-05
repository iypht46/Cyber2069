#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/TextRenderer.hpp"
#include "Utility/ObjectPool.h"
#include "HPsystem.hpp"
#include "EnemySpawner.hpp"
#include "PlayerController.hpp"

#include <map>
#include <vector>

class EnemySpawner;

enum POOL_TYPE {
	BULLET_MG = 0,
	ENEMY_FLYER,
	ENEMY_BOMBER
};

struct EnemyPreset {
	float FlyerRatio;
	float BomberRatio;
};

struct EnemyAmplifier {
	float FlyerHP;
	float FlyerSpeed;
	float FlyerDmg;

	float BomberHP;
	float BomberSpeed;
	float BomberDmg;
	float BomberAimTime;
	float BomberDashSpeed;
	float BomberExplodeDMG;
	float BomberExplodeRadius;

	float EnemySpawnRate;
};


class GameController : public BehaviourScript {
private:
	static GameController* instance;
	float ScoreValue = 0;
	float ComboValue = 1;

	float startHPscaleX;
	float startHPscaleY;

	float startStaminascaleX;
	float startStaminascaleY;

	float startHPposX;
	float startStaminaposX;
	
	PlayerController* player;
	HPsystem* PlayerHP;
	GameObject* HPbar;
	GameObject* Staminabar;
	GameObject* ScoreText;

	map<int, ObjectPool*> Pools;
	GameObject* FlyerSpawner;
	GameObject* BomberSpawner;

	vector<EnemySpawner*> Spawner;

	vector<EnemyPreset*> Preset;
	vector<EnemyAmplifier*> Amplifier;

	int currScoreCheckpoint = 0;

	float scoreCheckpoint[4] = { 0.0f, 10.0f,200.0f,300.0f };
	

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
	void updateStaminaUI();

	void updateSpawner();

	void AssignScoreText(GameObject* ScoreText);
	void AssignHPbar(GameObject* hpbar);
	void AssignStaminabar(GameObject* staminabar);
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