#pragma once
#include <memory>

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/TextRenderer.hpp"
#include "Utility/ObjectPool.h"
#include "HPsystem.hpp"
#include "EnemySpawner.hpp"
#include "PlayerController.hpp"
#include <memory>

#include <map>
#include <vector>

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class EnemySpawner;

enum POOL_TYPE {
	BULLET_MG = 0,
	BULLET_GL,
	BULLET_ZP,
	BULLET_BH,
	ENEMY_FLYER,
	ENEMY_BOMBER,
	ENEMY_TANK,
	ENEMY_CHARGER,
	ENEMY_SPITTER,
	ENEMY_QUEEN,
	BULLET_FUME
};

struct EnemyPreset {
	float FlyerRatio;
	float BomberRatio;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			FlyerRatio,
			BomberRatio
			);
	}
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

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			FlyerHP,
			FlyerSpeed,
			FlyerDmg,

			BomberHP,
			BomberSpeed,
			BomberDmg,
			BomberAimTime,
			BomberDashSpeed,
			BomberExplodeDMG,
			BomberExplodeRadius,

			EnemySpawnRate
			);
	}
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

	PlayerController* playerControl;
	HPsystem* PlayerHP;
	GameObject* HPbar;
	GameObject* Staminabar;
	GameObject* ScoreText;

	map<int, ObjectPool*> Pools; //assigned by user

	//created in constructor

	GameObject* FlyerSpawner; 
	GameObject* BomberSpawner;

	//extracted in on awake after created
	vector<EnemySpawner*> Spawner;

	vector<std::shared_ptr<EnemyPreset>> Preset;
	vector< std::shared_ptr<EnemyAmplifier>> Amplifier;

	EnemyPreset* CurrPreset;
	EnemyAmplifier* CurrAmplifier;

	int currScoreCheckpoint = 0;

	float scoreCheckpoint[4] = { 0.0f, 10.0f,200.0f,300.0f };

public:
	std::weak_ptr<GameObject> player;

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
	void updateStaminaUI();

	void updateSpawner();

	void AssignScoreText(GameObject* ScoreText);
	void AssignHPbar(GameObject* hpbar);
	void AssignStaminabar(GameObject* staminabar);
	void AssignPlayer(std::weak_ptr<GameObject> player);

	void AddPool(ObjectPool* pool, int type);
	ObjectPool* GetPool(int type);

	EnemyPreset* GetCurrPreset() { return CurrPreset; }
	EnemyAmplifier* GetCurrAmplifier() { return CurrAmplifier; }

	virtual void OnAwake();
	virtual void OnUpdate(float dt);

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			player,
			Preset,
			Amplifier,
			ScoreValue,
			ComboValue,
			startHPscaleX,
			startHPscaleY,
			startHPposX
			);
	}
};

CEREAL_REGISTER_TYPE(GameController);