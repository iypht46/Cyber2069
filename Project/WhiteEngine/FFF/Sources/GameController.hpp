#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/UIComponents/TextRenderer.hpp"
#include "Utility/ObjectPool.h"
#include "HPsystem.hpp"
#include "EnemySpawner.hpp"
#include "PlayerController.hpp"
#include "Scripts/GameControl/PlayerData.hpp"
#include "EquipmentManager.hpp"
#include "Core/Logger.hpp"

#include <memory>
#include <string>
#include <map>
#include <vector>

#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
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
	ENEMY_COCOON,
	BULLET_FUME
};

enum GAME_STATE {
	MAINMENU = 0,
	LOADOUT,
	GAMEPLAY,
	ENDING
};

enum GAMEPLAY_STATE {
	NORMAL = 0,
	QUEEN
};

struct EnemyPreset {
	float FlyerRatio = 0.4f;
	float BomberRatio = 0.2f;
	float QueenRatio = 0.0f;
	float CocoonRatio = 0.0f;
	float TankRatio = 0.1f;
	float ChargerRatio = 0.15f;
	float SpitterRatio = 0.15f;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			FlyerRatio,
			BomberRatio,
			QueenRatio,
			CocoonRatio,
			TankRatio,
			ChargerRatio,
			SpitterRatio
			);
	}
};

struct EnemyAmplifier {
	//flyer
	float FlyerHP = 1;
	float FlyerSpeed = 200;
	float FlyerDmg = 1;

	//bomber
	float BomberHP = 1;
	float BomberSpeed = 200;
	float BomberDmg = 0;
	float BomberAimTime = 1;
	float BomberDashSpeed = 600;
	float BomberExplodeDMG = 10;
	float BomberExplodeRadius = 200;

	//queen
	float QueenHP = 500;
	float QueenSpeed = 75;
	float QueenSpawnDelay = 0.1;

	//cocoon
	float CocoonHP = 10;

	//tank
	float TankSpeed = 50;
	float TankHP = 10;

	//charger
	float ChargerSpeed = 100;
	float ChargerHP = 2;
	float ChargerDashPauseTime = 1;
	float ChargerDashSpeed = 300;
	float ChragerDashDamage = 5;

	//spitter
	float SpitterSpeed = 0;
	float SpitterHP = 2;
	float SpitterFireRate = 1;

	float EnemySpawnRate = 3;

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

			QueenHP,
			QueenSpeed,
			QueenSpawnDelay,

			CocoonHP,

			TankSpeed,
			TankHP,

			ChargerSpeed,
			ChargerHP,
			ChargerDashPauseTime,
			ChargerDashSpeed,
			ChragerDashDamage,

			SpitterSpeed,
			SpitterHP,
			SpitterFireRate,

			EnemySpawnRate
			);
	}
};

class GameController : public BehaviourScript {
private:
	static GameController* instance;
	float ScoreValue = 0;
	float ComboValue = 1;

	std::unique_ptr<PlayerData> Data;

	float startHPscaleX;
	float startHPscaleY;

	float startStaminascaleX;
	float startStaminascaleY;

	float startHPposX;
	float startStaminaposX;

	PlayerController* playerControl;
	HPsystem* PlayerHP;

	glm::vec3 PlayerStartPosition;

	GameObject* Current_Queen = nullptr;
	GameObject* Current_Cocoon = nullptr;

	int CocoonNeed = 5;
	int CocoonCount = 0;

	map<int, ObjectPool*> Pools; //added while onawake and init manually, hard coding, collect all pool used in game

	//extracted in on awake after created, need to assign since the editor or don't bother create any object
	vector<EnemySpawner*> Spawners;
	EnemySpawner* QueenSpawner;
	EnemySpawner* CocoonSpawner;

	vector<std::shared_ptr<EnemyPreset>> Presets;
	vector<std::shared_ptr<EnemyAmplifier>> Amplifiers;

	EnemyPreset* CurrPreset;
	EnemyAmplifier* CurrAmplifier;

	int currScoreCheckpoint = 0;

	float scoreCheckpoint[4] = { 0.0f, 10.0f,200.0f,300.0f };

	void updateHPui();
	void updateStaminaUI();

	void updateSpawner();

	//create on runtime, it will generate objects and init them
	void CreatePool(std::string prefabPath, int poolType, int poolSize);

	//create enemy spawner and assign to gamecontroller, 
	//*NOT set the spawn range
	EnemySpawner* CreateSpawner(int enemyType);

	bool CursedMode = false;

	int CurrentState = MAINMENU;
	int CurrentGameplayState = NORMAL;

	int NextState = MAINMENU;
	int NextGameplayState = NORMAL;

	bool StateChanged = false;
	bool StateGamplayChanged = false;

	//player data manager
	void LoadData();
	void SaveData();

public:
	std::weak_ptr<GameObject> player;
	std::weak_ptr<GameObject> HPbar;
	std::weak_ptr<GameObject> Staminabar;
	std::weak_ptr<GameObject> ScoreText;
	std::weak_ptr<GameObject> ComboText;

	std::weak_ptr<GameObject> loadoutUI;

	GameController();
	~GameController() {}

	static GameController* GetInstance();
	float GetScore();
	float GetCombo();

	void SetScore(float score);

	void AddScoreValue(float baseScore);
	void AddComboValue(float value);

	void SetCombo(float combo);

	void ResetScore();

	GameObject* SpawnQueen();
	GameObject* SpawnCocoon();

	void AssignPlayer(std::weak_ptr<GameObject> player);

	void AddPool(ObjectPool* pool, int type);
	ObjectPool* GetPool(int type);

	void SetActiveAllObjectInPool(bool active);

	EnemyPreset* GetCurrPreset() { return CurrPreset; }
	EnemyAmplifier* GetCurrAmplifier() { return CurrAmplifier; }

	bool isCursedMode() { return CursedMode; }
	void SetCursedMode(bool mode) { this->CursedMode = mode; }

	void AddSpawner(EnemySpawner* spawner);
	EnemySpawner* GetSpawner(int pooltype);
	void SetSpawningAllSpawner(bool spawning);

	int GetGameState() { return CurrentState; }
	int GetGameplayState() { return CurrentGameplayState; }

	void SetGameState(int state) { this->NextState = state; }
	void SetGameplayState(int state) { this->NextGameplayState = state; }

	GameObject* GetPlayer() { return playerControl->GetGameObject(); }

	virtual void OnAwake() override;
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			cereal::defer(player),
			cereal::defer(HPbar),
			cereal::defer(Staminabar),
			cereal::defer(ScoreText),
			cereal::defer(ComboText),
			Presets,
			Amplifiers,
			ScoreValue,
			ComboValue,
			startHPscaleX,
			startHPscaleY,
			startHPposX
			);

		ENGINE_INFO("Finished");
	}
};

CEREAL_REGISTER_TYPE(GameController);