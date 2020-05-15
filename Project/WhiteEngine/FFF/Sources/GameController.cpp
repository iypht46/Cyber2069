#include "GameController.hpp"
#include "Scripts/GameControl/UIController.h"
#include "Scripts/GameControl/SoundtrackController.h"
#include "Scripts/GameControl/CameraController.h"
#include "Scripts/Misc/TargetArrow.h"

#include "Core/GameInfo.h"

#include "Input/Input.hpp"

GameController* GameController::instance = nullptr;
float GameController::stateChangeDelay = 3.0f;

GameController::GameController() {
	MenuCamPos = glm::vec3(0, 2000, 0);

	if (instance == nullptr) 
	{
		instance = this;
	}
}

GameController* GameController::GetInstance() 
{
	if (instance != nullptr) 
	{
		return instance;
	}
	else {
		return nullptr;
	}
}

void GameController::OnAwake() {
	this->PlayerHP = player.lock()->GetComponent<HPsystem>();
	this->playerControl = player.lock()->GetComponent<PlayerController>();
	this->sp = m_gameObject->GetComponent<SoundPlayer>();
	this->soundtrackCon = m_gameObject->GetComponent<SoundtrackController>();

	LoadGameConfig();

	LoadData();
}

void GameController::OnStart() {
	ENGINE_INFO("GameControl Creating Bullets");
	ENGINE_INFO("======================================================================");
	{
		CreatePool(PrefabPath("Bullet_MG"), POOL_TYPE::BULLET_MG, 20);
		CreatePool(PrefabPath("Bullet_GL"), POOL_TYPE::BULLET_GL, 20);
		CreatePool(PrefabPath("Bullet_ZP"), POOL_TYPE::BULLET_ZP, 20);
		CreatePool(PrefabPath("Bullet_BH"), POOL_TYPE::BULLET_BH, 20);
		CreatePool(PrefabPath("Bullet_Fume"), POOL_TYPE::BULLET_FUME, 20);
	}
	
	ENGINE_INFO("GameControl Creating Items");
	ENGINE_INFO("======================================================================");
	{
		CreatePool(PrefabPath("ItemDrop"), POOL_TYPE::ITEM_DROP, 2);
	}

	ENGINE_INFO("GameControl Creating Enemies");
	ENGINE_INFO("======================================================================");
	{
		//flyer spawner
		ENGINE_INFO("GameControl Creating Flyer");
		CreatePool(PrefabPath("Flyer"), POOL_TYPE::ENEMY_FLYER, 50);
		//CreateSpawner(POOL_TYPE::ENEMY_FLYER)->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);
		CreateSpawner(POOL_TYPE::ENEMY_FLYER)->SetSpawnMode(SPAWN_MODE::EDGE);

		//bomber spawner
		ENGINE_INFO("GameControl Creating Bomber");
		CreatePool(PrefabPath("Bomber"), POOL_TYPE::ENEMY_BOMBER, 50);
		//CreateSpawner(POOL_TYPE::ENEMY_BOMBER)->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);
		CreateSpawner(POOL_TYPE::ENEMY_BOMBER)->SetSpawnMode(SPAWN_MODE::EDGE);

		//Tank
		ENGINE_INFO("GameControl Creating Tank");
		CreatePool(PrefabPath("Tank"), POOL_TYPE::ENEMY_TANK, 10);
		//CreateSpawner(POOL_TYPE::ENEMY_TANK)->SetSpawnRange(0, 100, 0, 100);
		CreateSpawner(POOL_TYPE::ENEMY_TANK)->SetSpawnMode(SPAWN_MODE::PLATFORM);

		//Charger
		ENGINE_INFO("GameControl Creating Charger");
		CreatePool(PrefabPath("Charger"), POOL_TYPE::ENEMY_CHARGER, 10);
		//CreateSpawner(POOL_TYPE::ENEMY_CHARGER)->SetSpawnRange(0, 100, 0, 100);
		CreateSpawner(POOL_TYPE::ENEMY_CHARGER)->SetSpawnMode(SPAWN_MODE::PLATFORM);

		//Spitter
		ENGINE_INFO("GameControl Creating Spitter");
		CreatePool(PrefabPath("Spitter"), POOL_TYPE::ENEMY_SPITTER, 10);
		//CreateSpawner(POOL_TYPE::ENEMY_SPITTER)->SetSpawnRange(-400, 100, 400, 100);
		CreateSpawner(POOL_TYPE::ENEMY_SPITTER)->SetSpawnMode(SPAWN_MODE::PLATFORM);

		//Queen spawner
		ENGINE_INFO("GameControl Creating Queen");
		CreatePool(PrefabPath("Queen"), POOL_TYPE::ENEMY_QUEEN, 1);
		QueenSpawner = CreateSpawner(POOL_TYPE::ENEMY_QUEEN);
		QueenSpawner->SetSpawnMode(SPAWN_MODE::RANGE);
		QueenSpawner->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() * 2, Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() * 2);

		//Cocoon spawner
		ENGINE_INFO("GameControl Creating Queen");
		CreatePool(PrefabPath("Cocoon"), POOL_TYPE::ENEMY_COCOON, 1);
		CocoonSpawner = CreateSpawner(POOL_TYPE::ENEMY_COCOON);
		CocoonSpawner->SetSpawnMode(SPAWN_MODE::PLATFORM);
	}

	ENGINE_INFO("GameControl Creating Particles");
	ENGINE_INFO("======================================================================");
	{
		//player
		CreatePool(ParticlePrefabPath("PTCL_Player_Dash"), POOL_TYPE::PTCL_PLAYER_DASH, 1);
		CreatePool(ParticlePrefabPath("PTCL_Player_Jump"), POOL_TYPE::PTCL_PLAYER_JUMP, 3);
		CreatePool(ParticlePrefabPath("PTCL_Player_Killed"), POOL_TYPE::PTCL_PLAYER_KILLED, 1);

		//weapon
		CreatePool(ParticlePrefabPath("PTCL_Blackhole"), POOL_TYPE::PTCL_BLACKHOLE, 5);
		CreatePool(ParticlePrefabPath("PTCL_Bullet_Hit_Armor"), POOL_TYPE::PTCL_BULLET_HIT_ARMOR, 15);
		CreatePool(ParticlePrefabPath("PTCL_Bullet_Hit_SoftBody"), POOL_TYPE::PTCL_BULLET_HIT_ENEMY, 15);
		CreatePool(ParticlePrefabPath("PTCL_Zapper_Zap"), POOL_TYPE::PTCL_ZAPPER_ZAP, 6);
		CreatePool(ParticlePrefabPath("PTCL_Laser_Hit"), POOL_TYPE::PTCL_LASER_HIT, 10);
		CreatePool(ParticlePrefabPath("PTCL_Explosion_Grenade"), POOL_TYPE::PTCL_EXPLOSION_GRENADE, 5);

		//Item
		CreatePool(ParticlePrefabPath("PTCL_Equipment_Pickup"), POOL_TYPE::PTCL_PICKUP_EQUIPMENT, 1);
		CreatePool(ParticlePrefabPath("PTCL_Heal_Pickup"), POOL_TYPE::PTCL_PICKUP_HEAL, 1);

		//Enemy
		CreatePool(ParticlePrefabPath("PTCL_Explosion_Bomber"), POOL_TYPE::PTCL_EXPLOSION_BOMBER, 15);
		CreatePool(ParticlePrefabPath("PTCL_Explosion_Shrapnel"), POOL_TYPE::PTCL_EXPLOSION_SHRAPNEL, 1);
		CreatePool(ParticlePrefabPath("PTCL_Explosion_Spitter"), POOL_TYPE::PTCL_EXPLOSION_SPITTER, 15);
		CreatePool(ParticlePrefabPath("PTCL_Killed_Cocoon"), POOL_TYPE::PTCL_KILLED_COCOON, 1);
		CreatePool(ParticlePrefabPath("PTCL_Killed_Enemy"), POOL_TYPE::PTCL_KILLED_ENEMY, 20);
		CreatePool(ParticlePrefabPath("PTCL_Killed_Queen_Fluid"), POOL_TYPE::PTCL_KILLED_QUEEN_FLUID, 2);
		CreatePool(ParticlePrefabPath("PTCL_Killed_Queen_Smoke"), POOL_TYPE::PTCL_KILLED_QUEEN_SMOKE, 1);
		CreatePool(ParticlePrefabPath("PTCL_QueenSpawn1"), POOL_TYPE::PTCL_ENEMYSPAWN1, 10);
		CreatePool(ParticlePrefabPath("PTCL_QueenSpawn2"), POOL_TYPE::PTCL_ENEMYSPAWN2, 10);
	}

	//difficulty ting needs to go out side---------------------------
	/*shared_ptr<EnemyAmplifier> a = std::make_shared<EnemyAmplifier>();
	a->EnemySpawnRate = 1.0f;
	a->FlyerHP = 1.0f;
	a->FlyerSpeed = 100.0f;
	a->FlyerDmg = 10.0f;

	a->BomberHP = 1.0f;
	a->BomberSpeed = 300.0f;
	a->BomberDmg = 10.0f;
	a->BomberAimTime = 0.5f;
	a->BomberDashSpeed = 700.0f;
	a->BomberExplodeDMG = 30.0f;
	a->BomberExplodeRadius = 200.0f;

	Amplifiers.push_back(a);

	a = std::make_shared<EnemyAmplifier>();
	a->EnemySpawnRate = 2.0f;
	a->FlyerHP = 1.0f;
	a->FlyerSpeed = 300.0f;
	a->FlyerDmg = 10.0f;

	a->BomberHP = 1.0f;
	a->BomberSpeed = 300.0f;
	a->BomberDmg = 10.0f;
	a->BomberAimTime = 0.2f;
	a->BomberDashSpeed = 700.0f;
	a->BomberExplodeDMG = 30.0f;
	a->BomberExplodeRadius = 200.0f;

	Amplifiers.push_back(a);

	std::shared_ptr<EnemyPreset> p1 = std::make_shared<EnemyPreset>();
	Presets.push_back(p1);
	//------------------------------------------------------------

	CurrAmplifier = Amplifiers[0].get();
	CurrPreset = Presets[0].get();
	*/

	SetSpawningAllSpawner(false);
	playerControl->GetGameObject()->SetActive(false);

	//loadoutUI.lock()->SetActive(false);

	PlayerStartPosition = playerControl->GetGameObject()->m_transform->GetPosition();

	m_gameObject->GetComponent<EquipmentManager>()->Unlock_WEAPON(WEAPON_TYPE::WEAPON_MACHINEGUN);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_WEAPON(WEAPON_TYPE::WEAPON_LASER);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_WEAPON(WEAPON_TYPE::WEAPON_GRENADELAUNCHER);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_WEAPON(WEAPON_TYPE::WEAPON_ZAPPER);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_WEAPON(WEAPON_TYPE::WEAPON_BLACKHOLE);

	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_BULLETAMP);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_FIRERATEUP);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_SPEEDRUNNER);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_ATKUP);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_LOWGRAV);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_ARTIFACTAMP);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_CURSEDPENDANT);

	StateChanged = true;

	Graphic::getCamera()->SetPos(MenuCamPos);
}

void GameController::CreatePool(std::string prefabPath, int poolType, int poolSize) {
	ObjectPool* Pool = new ObjectPool();
	Pool->prefabObjectPath = prefabPath;
	Pool->objectCount = poolSize;
	Pool->Init();
	AddPool(Pool, poolType);
}

EnemySpawner* GameController::CreateSpawner(int enemyType) {
	EnemySpawner* Spawner = new EnemySpawner();
	Spawner->EnemyTarget = player.lock().get();
	Spawner->SetSpawnType(enemyType);

	Spawners.push_back(Spawner);
	return Spawner;
}

void GameController::OnUpdate(float dt)
{
	//update enemy spawner, since they're not created in system
	for (EnemySpawner* sp: Spawners) {
		sp->OnUpdate(dt);
	}

	if (CurrentState != NextState) 
	{
		switch (NextState)
		{
		case GAMEPLAY:
			gameStateChangeTimer += dt;
			break;

		case MAINMENU:
		case LOADOUT:
		case QUIT:
		case ENDING:
			gameStateChangeTimer = stateChangeDelay;
			break;

		default:
			break;
		}

		if (gameStateChangeTimer >= stateChangeDelay) {
			CurrentState = NextState;
			StateChanged = true;
		}
	}

	if (CurrentGameplayState != NextGameplayState)
	{
		switch (NextGameplayState)
		{
		case NORMAL:
			gameplayStateChangeTimer += dt;
			break;

		case QUEEN:
			gameplayStateChangeTimer += dt;
			break;

		default:
			break;
		}

		if (gameplayStateChangeTimer >= stateChangeDelay) {
			CurrentGameplayState = NextGameplayState;
			StateGamplayChanged = true;
		}
	}

	switch (CurrentState)
	{
	case GAME_STATE::MAINMENU:
		//Do only once after state changed
		if (StateChanged) 
		{
			CameraController::GetInstance()->SetTarget(MenuCamPos);
			UIController::GetInstance()->ToggleUI(UI_GROUP::MainMenu);
			soundtrackCon->PlayState(SOUNDTRACK_STATE::MENU, true);

			UIController::GetInstance()->UpdateVolumeTexts();

			this->GetGameObject()->GetComponent<EquipmentManager>()->ResetPlayerEquipment();

			SetSpawningAllSpawner(false);
			SetActiveAllObjectInPool(false);
			playerControl->GetGameObject()->SetActive(false);

			//loadoutUI.lock()->SetActive(false);

			Restart();

			SaveGameConfig();

			StateChanged = false;
		}

		if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE)) 
		{
			SetGameState(GAME_STATE::LOADOUT);
		}

		if (Input::GetKeyDown(Input::KeyCode::KEY_S))
		{
			SaveData();
		}

		if (Input::GetKeyDown(Input::KeyCode::KEY_L))
		{
			Data->PrintLeaderboard();
		}

		break;
	case GAME_STATE::LOADOUT:
		//Do only once after state changed
		if (StateChanged)
		{
			CameraController::GetInstance()->SetTarget(MenuCamPos);
			UIController::GetInstance()->ToggleUI(UI_GROUP::Loadout);

			//loadoutUI.lock()->SetActive(true);

			StateChanged = false;
		}

		break;
	case GAME_STATE::GAMEPLAY:
		//Do only once after state changed
		if (StateChanged) 
		{
			//CameraController::GetInstance()->SetTarget((player.lock()->m_transform).get());

			//UIController::GetInstance()->ToggleUI(UI_GROUP::Gameplay);

			//sp->SetSound(SoundPath("SFX_Game_SessionStart"));
			//sp->PlaySound();

			Restart();
			//playerControl->GetGameObject()->SetActive(true);
			//playerControl->GetGameObject()->m_transform->SetPosition(PlayerStartPosition);

			//loadoutUI.lock()->SetActive(false);

			StateChanged = false;
			StateGamplayChanged = true;

			//this->GetGameObject()->GetComponent<EquipmentManager>()->InitPlayerEquipment();
		}

		SetSpawningAllSpawner(true);

		UIController::GetInstance()->UpdateEquipmentDisplay();
		UIController::GetInstance()->updateHPUI();
		UIController::GetInstance()->updateStaminaUI();
		UIController::GetInstance()->updateScoreUI();

		if (Input::GetKeyDown(Input::KeyCode::KEY_ESCAPE) && !pause)
		{
			pause = true;

			UIController::GetInstance()->ToggleUI(UI_GROUP::Pause);

			if (Current_Queen == nullptr)
			{
				UIController::GetInstance()->SetActiveQueenUI(false);
			}
		}
		else if (Input::GetKeyDown(Input::KeyCode::KEY_ESCAPE) && pause) 
		{
			pause = false;
			UIController::GetInstance()->ToggleUI(UI_GROUP::Gameplay);

			if (Current_Queen == nullptr)
			{
				UIController::GetInstance()->SetActiveQueenUI(false);
			}
		}

		switch (CurrentGameplayState)
		{
		case GAMEPLAY_STATE::NORMAL:
			//Do only once after gameplaystate changed
			if (StateGamplayChanged) 
			{
				//Spawn Cocoon
				if (CocoonSpawner != nullptr)
				{
					Current_Cocoon = CocoonSpawner->SpawnEnemy();
					cocoonarrow.lock()->target = Current_Cocoon->m_transform.get();
				}

				//soundtrackCon->Stop(true);
				soundtrackCon->PlayState(SOUNDTRACK_STATE::GAMEPLAY_NORMAL, true);

				UIController::GetInstance()->SetActiveQueenUI(false);

				StateGamplayChanged = false;
			}
			
			if (Current_Cocoon != nullptr) 
			{
				//if cocoon dead plus score and spawn a new one
				if (!Current_Cocoon->Active())
				{
					CocoonCount++;

					if (CocoonCount == CurrAmplifier->CocoonNeeded)
					{
						CocoonCount = 0;
						SetGameplayState(GAMEPLAY_STATE::QUEEN);

						soundtrackCon->Stop(true);

						//StateGamplayChanged = true;
					}
					else if (NextGameplayState != GAMEPLAY_STATE::QUEEN)
					{
						Current_Cocoon = CocoonSpawner->SpawnEnemy();
						cocoonarrow.lock()->target = Current_Cocoon->m_transform.get();
					}
				}
			}
			else {
				
				if (CocoonSpawner != nullptr)
				{
					Current_Cocoon = CocoonSpawner->SpawnEnemy();
					cocoonarrow.lock()->target = Current_Cocoon->m_transform.get();
				}
			}

			break;
		case GAMEPLAY_STATE::QUEEN:
			//Do only once after gameplaystate changed
			if (StateGamplayChanged) 
			{
				Current_Queen = SpawnQueen();
				cocoonarrow.lock()->target = Current_Queen->m_transform.get();

				UIController::GetInstance()->QueenHP = Current_Queen->GetComponent<HPsystem>();
				UIController::GetInstance()->SetActiveQueenUI(true);

				soundtrackCon->PlayState(SOUNDTRACK_STATE::GAMEPLAY_BOSS, true);

				StateGamplayChanged = false;
			}

			if (Current_Queen != nullptr) 
			{
				UIController::GetInstance()->updateQueenHPUI();

				//if Queen Dead go back to normal state
				if (!Current_Queen->Active()) 
				{
					soundtrackCon->Stop(true);
					SetGameplayState(GAMEPLAY_STATE::NORMAL);
				}
			}


			break;
		default:
			break;
		}


		if (!playerControl->GetGameObject()->Active()) 
		{
			CocoonCount = 0;
			SetGameplayState(GAMEPLAY_STATE::NORMAL);
			SetGameState(GAME_STATE::ENDING);
		}

		break;
	case GAME_STATE::ENDING:
		//Do only once after state changed
		if (StateChanged) 
		{
			sp->SetSound(SoundPath("SFX_Game_GameOver"));
			sp->PlaySound();
			
			//update score
			Data->AddLeaderboardEntry("whoite", ScoreValue);

			CameraController::GetInstance()->StopFollowing();
			UIController::GetInstance()->ToggleUI(UI_GROUP::GameOver);

			SetSpawningAllSpawner(false);
			SetActiveAllObjectInPool(false);
			playerControl->GetGameObject()->SetActive(false);

			soundtrackCon->Stop(false);

			string scoreText = "Score: ";

			scoreText += to_string((int)ScoreValue);
			UIController::GetInstance()->GameOverScoreText.lock()->GetComponent<TextRenderer>()->SetText(scoreText);

			Restart();

			SaveData();

			StateChanged = false;

		}

		if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
		{
			SetGameState(GAME_STATE::MAINMENU);
		}

		break;
	case GAME_STATE::QUIT:
		//Do only once after state changed
		if (StateChanged)
		{
			GAME_INFO("QUIT");

			SaveData();
			SaveGameConfig();

			World::GameInfo* g_gameInfo;
			g_gameInfo = &(World::GameInfo::GetInstance());

			g_gameInfo->GameShouldClose();
			Graphic::Window::SetWindowShouldClose(true);

			StateChanged = false;
		}

		break;
	default:
		break;
	}
}

GameObject* GameController::SpawnQueen() {
	if (QueenSpawner != nullptr) {
		return QueenSpawner->SpawnEnemy();
	}

	return nullptr;
}

GameObject* GameController::SpawnCocoon() {
	if (CocoonSpawner != nullptr) {
		return CocoonSpawner->SpawnEnemy();
	}

	return nullptr;
}

float GameController::GetScore() {
	return this->ScoreValue;
}

float GameController::GetCombo() {
	return this->ComboValue;
}

void GameController::SetScore(float score) {
	this->ScoreValue = score;
}

void GameController::AddScoreValue(float baseScore) {
	this->ScoreValue += baseScore * ComboValue;

	updateDifficulty();
}

void GameController::AddComboValue(float value) {
	this->ComboValue += value;
}

void GameController::SetCombo(float combo) {
	ComboValue = combo;
}

void GameController::ResetCombo() {
	this->ComboValue = 1;
	sp->SetSound(SoundPath("SFX_Game_LoseCombo"));
	sp->PlaySound();
}

void GameController::Restart() {
	this->ScoreValue = 0;
	this->ComboValue = 1;

	updateDifficulty();
	updateEnemyPreset();

	updateSpawner();
}

void GameController::AssignPlayer(std::weak_ptr<GameObject> player) {
	this->player = player;
}

void GameController::AddPool(ObjectPool* pool, int type)
{
	Pools[type] = pool;
}

ObjectPool* GameController::GetPool(int type) {
	return Pools[type];
}

void GameController::updateDifficulty() 
{
	CurrAmplifier = Amplifiers[0].get();

	for (std::shared_ptr<EnemyAmplifier> amp : Amplifiers) {
		if (ScoreValue >= amp->RequiredScore) {
			CurrAmplifier = amp.get();
		}
		else {
			break;
		}
	}

	ENGINE_INFO("update difficulty");
}

void GameController::updateEnemyPreset()
{
	CurrPreset = Presets[0].get();

	int randPreset = rand() % Presets.size();

	CurrPreset = Presets[randPreset].get();

	ENGINE_INFO("update preset");
}

void GameController::SpawnEnemies() {
	for (EnemySpawner* spawner : Spawners) {
		spawner->SpawnEnemy();
	}
}

void GameController::updateSpawner()
{
	for (EnemySpawner* spawner : Spawners) {
		spawner->SpawnAmplifier = CurrAmplifier;
		spawner->SpawnPreset = CurrPreset;

		spawner->updateSpawner();
	}
}

void GameController::AddSpawner(EnemySpawner* spawner) 
{
	Spawners.push_back(spawner);
}

EnemySpawner* GameController::GetSpawner(int pooltype) 
{
	for (EnemySpawner* e : Spawners)
	{
		if (e->GetType() == pooltype) 
		{
			return e;
		}
	}

	return nullptr;
}

void GameController::SetSpawningAllSpawner(bool spawning)
{
	for (EnemySpawner* e : Spawners) 
	{
		e->SetSpawning(spawning);
	}
}

void GameController::SetActiveAllObjectInPool(bool active) 
{
	for (pair<int, ObjectPool*> pool : Pools) 
	{
		pool.second->SetActiveAllGameObject(active);
	}
}

void GameController::SetGameState(int state) {
	if (NextState != state) {
		this->NextState = state;

		switch (NextState)
		{
		case MAINMENU:
			gameStateChangeTimer = stateChangeDelay;
			break;
		case LOADOUT:
			gameStateChangeTimer = stateChangeDelay;
			break;
		case GAMEPLAY:
			gameStateChangeTimer = 0;

			CameraController::GetInstance()->SetTarget((player.lock()->m_transform).get());
			UIController::GetInstance()->ToggleUI(UI_GROUP::Gameplay);
			UIController::GetInstance()->SetActiveQueenUI(false);

			sp->SetSound(SoundPath("SFX_Game_SessionStart"));
			sp->PlaySound();

			soundtrackCon->Stop(true);

			ScoreValue = 0;
			playerControl->GetGameObject()->SetActive(true);
			playerControl->GetGameObject()->m_transform->SetPosition(PlayerStartPosition);

			this->GetGameObject()->GetComponent<EquipmentManager>()->InitPlayerEquipment();

			break;
		case ENDING:
			gameStateChangeTimer = stateChangeDelay;
			break;
		default:
			break;
		}
	}
}

void GameController::SetGameplayState(int state){
	if (NextGameplayState != state) {
		this->NextGameplayState = state;

		switch (NextGameplayState)
		{
		case NORMAL:
			gameplayStateChangeTimer = 0;
			break;
		case QUEEN:
			gameplayStateChangeTimer = 0;
			break;
		default:
			break;
		}
	}
}

void GameController::LoadData() {
	EquipmentManager* equipmentManager = m_gameObject->GetComponent<EquipmentManager>();
	if (equipmentManager != nullptr) {
		Data = std::make_unique<PlayerData>();
		Serialization::LoadObject(*Data.get(), DataPath("PlayerData"));

		equipmentManager->SetWeaponUnlockData(Data->Weapons);
		equipmentManager->SetArtifactUnlockData(Data->Artifacts);
	}
	else {
		ENGINE_ERROR("No Equipment Mannager Found In GameController");
	}
}

void GameController::SaveData() {
	EquipmentManager* equipmentManager = m_gameObject->GetComponent<EquipmentManager>();
	Data->Weapons = equipmentManager->Unlock_Weapons;
	Data->Artifacts = equipmentManager->Unlock_Artifacts;
	
	Serialization::SaveObject(*Data, DataPath("PlayerData"));
}

void GameController::ResetData() {
	Data->ResetProgress();
	Data->ResetLeaderboard();
}

void GameController::ResetPlayerProgress() 
{
	EquipmentManager* equipmentManager = m_gameObject->GetComponent<EquipmentManager>();
	ResetData();

	equipmentManager->Unlock_Weapons = Data->Weapons;
	equipmentManager->Unlock_Artifacts = Data->Artifacts;

	SaveData();

}

void GameController::LoadGameConfig() {
	std::unique_ptr<GameConfig> config = std::make_unique<GameConfig>();
	Serialization::LoadObjectXML(*config, XMLConfigPath("gameconfig"));

	Amplifiers = config->Amplifiers;
	Presets = config->Presets;

	SoundPlayer::SetMasterVolume(config->MasterVolume);
	SoundPlayer::SetMusicVolume(config->MusicVolume);
	SoundPlayer::SetSFXVolume(config->SFXVolume);
}

void GameController::SaveGameConfig() {
	std::unique_ptr<GameConfig> config = std::make_unique<GameConfig>();

	config->Amplifiers = Amplifiers;
	config->Presets = Presets;

	config->MasterVolume = SoundPlayer::GetMasterVolume();
	config->MusicVolume = SoundPlayer::GetMusicVolume();
	config->SFXVolume = SoundPlayer::GetSFXVolume();

	Serialization::SaveObjectXML(*config, XMLConfigPath("gameconfig"));
}
