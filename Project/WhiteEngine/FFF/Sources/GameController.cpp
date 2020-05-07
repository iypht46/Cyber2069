#include "GameController.hpp"
#include "Input/Input.hpp"

GameController* GameController::instance = nullptr;

GameController::GameController() {
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

	startHPscaleX = HPbar.lock()->m_transform->GetScale().x;
	startHPscaleY = HPbar.lock()->m_transform->GetScale().y;
	startHPposX = HPbar.lock()->m_transform->GetPosition().x;

	startStaminascaleX = Staminabar.lock()->m_transform->GetScale().x;
	startStaminascaleY = Staminabar.lock()->m_transform->GetScale().y;
	startStaminaposX = Staminabar.lock()->m_transform->GetPosition().x;

	LoadData();
}

void GameController::OnStart() {
	ENGINE_INFO("GameControl Creating Bullets");
	ENGINE_INFO("======================================================================");
	CreatePool(PrefabPath("Bullet_MG"), POOL_TYPE::BULLET_MG, 20);
	CreatePool(PrefabPath("Bullet_GL"), POOL_TYPE::BULLET_GL, 20);
	CreatePool(PrefabPath("Bullet_ZP"), POOL_TYPE::BULLET_ZP, 20);
	CreatePool(PrefabPath("Bullet_BH"), POOL_TYPE::BULLET_BH, 20);
	CreatePool(PrefabPath("Bullet_Fume"), POOL_TYPE::BULLET_FUME, 20);
	
	ENGINE_INFO("GameControl Creating Item");
	ENGINE_INFO("======================================================================");
	CreatePool(PrefabPath("ItemDrop"), POOL_TYPE::ITEM_DROP, 1);

	ENGINE_INFO("GameControl Creating Enemy");
	ENGINE_INFO("======================================================================");

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
	QueenSpawner->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() * 0.75, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() * 0.75);

	//Cocoon spawner
	ENGINE_INFO("GameControl Creating Queen");
	CreatePool(PrefabPath("Cocoon"), POOL_TYPE::ENEMY_COCOON, 1);
	CocoonSpawner = CreateSpawner(POOL_TYPE::ENEMY_COCOON);
	CocoonSpawner->SetSpawnMode(SPAWN_MODE::PLATFORM);



	//difficulty ting needs to go out side---------------------------
	shared_ptr<EnemyAmplifier> a = std::make_shared<EnemyAmplifier>();
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

	SetSpawningAllSpawner(false);
	playerControl->GetGameObject()->SetActive(false);
	HPbar.lock()->SetActive(false);
	Staminabar.lock()->SetActive(false);
	ScoreText.lock()->SetActive(false);
	ComboText.lock()->SetActive(false);

	loadoutUI.lock()->SetActive(false);

	PlayerStartPosition = playerControl->GetGameObject()->m_transform->GetPosition();

	m_gameObject->GetComponent<EquipmentManager>()->Unlock_WEAPON(WEAPON_TYPE::WEAPON_MACHINEGUN);

	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_BULLETAMP);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_FIRERATEUP);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_SPEEDRUNNER);
	m_gameObject->GetComponent<EquipmentManager>()->Unlock_ARTIFACT(ARTIFACT_TYPE::ARTF_ATKUP);

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
		CurrentState = NextState;
		StateChanged = true;
	}

	if (CurrentGameplayState != NextGameplayState)
	{
		CurrentGameplayState = NextGameplayState;
		StateGamplayChanged = true;
	}

	switch (CurrentState)
	{
	case GAME_STATE::MAINMENU:
		//Do only once after state changed
		if (StateChanged) 
		{
			playerControl->GetGameObject()->SetActive(false);
			HPbar.lock()->SetActive(false);
			Staminabar.lock()->SetActive(false);
			ScoreText.lock()->SetActive(false);
			ComboText.lock()->SetActive(false);

			loadoutUI.lock()->SetActive(false);

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
			loadoutUI.lock()->SetActive(true);

			StateChanged = false;
		}

		break;
	case GAME_STATE::GAMEPLAY:
		//Do only once after state changed
		if (StateChanged) 
		{
			ScoreValue = 0;
			playerControl->GetGameObject()->SetActive(true);
			playerControl->GetGameObject()->m_transform->SetPosition(PlayerStartPosition);

			HPbar.lock()->SetActive(true);
			Staminabar.lock()->SetActive(true);
			ScoreText.lock()->SetActive(true);
			ComboText.lock()->SetActive(true);

			loadoutUI.lock()->SetActive(false);

			StateChanged = false;
			StateGamplayChanged = true;
		}

		this->ScoreText.lock()->GetComponent<TextRenderer>()->SetText("Score: " + to_string((int)ScoreValue));
		this->ComboText.lock()->GetComponent<TextRenderer>()->SetText("x " + to_string((int)ComboValue));

		SetSpawningAllSpawner(true);

		updateHPui();
		updateStaminaUI();
		updateSpawner();

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
				}

				StateGamplayChanged = false;
			}
			
			if (Current_Cocoon != nullptr) 
			{
				//if cocoon dead plus score and spawn a new one
				if (!Current_Cocoon->Active())
				{
					CocoonCount++;

					if (CocoonCount == CocoonNeed)
					{
						CocoonCount = 0;
						SetGameplayState(GAMEPLAY_STATE::QUEEN);
					}
					else 
					{
						Current_Cocoon = CocoonSpawner->SpawnEnemy();
					}
				}
			}
			else {
				
				if (CocoonSpawner != nullptr)
				{
					Current_Cocoon = CocoonSpawner->SpawnEnemy();
				}
			}

			break;
		case GAMEPLAY_STATE::QUEEN:
			//Do only once after gameplaystate changed
			if (StateGamplayChanged) 
			{
				//Spawn Queen
				if (QueenSpawner != nullptr)
				{
					Current_Queen = QueenSpawner->SpawnEnemy();
				}

				StateGamplayChanged = false;
			}

			if (Current_Queen != nullptr) 
			{
				//if Queen Dead go back to normal state
				if (!Current_Queen->Active()) 
				{
					SetGameplayState(GAMEPLAY_STATE::NORMAL);
				}
			}

			break;
		default:
			break;
		}


		if (playerControl->GetGameObject()->GetComponent<HPsystem>()->isDead()) 
		{
			this->GetGameObject()->GetComponent<EquipmentManager>()->ResetPlayerEquipment();
			
			CocoonCount = 0;
			SetGameplayState(GAMEPLAY_STATE::NORMAL);
			SetGameState(GAME_STATE::ENDING);
		}

		break;
	case GAME_STATE::ENDING:
		//Do only once after state changed
		if (StateChanged) 
		{
			SetSpawningAllSpawner(false);
			SetActiveAllObjectInPool(false);
			playerControl->GetGameObject()->SetActive(false);
			HPbar.lock()->SetActive(false);
			Staminabar.lock()->SetActive(false);
			ScoreText.lock()->SetActive(false);
			ComboText.lock()->SetActive(false);

			currScoreCheckpoint = 0;
			
			//update score
			Data->AddLeaderboardEntry("whoite", ScoreValue);

			StateChanged = false;
		}

		if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
		{
			SetGameState(GAME_STATE::MAINMENU);
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
}

void GameController::AddComboValue(float value) {
	this->ComboValue += value;
}

void GameController::SetCombo(float combo) {
	ComboValue = combo;
}

void GameController::ResetScore() {
	this->ScoreValue = 0;
	this->ComboValue = 1;
}

void GameController::AssignPlayer(std::weak_ptr<GameObject> player) {
	this->player = player;
}

void GameController::updateHPui() {
	float playerHP = PlayerHP->GetHP();

	if (playerHP < 0)
	{
		playerHP = 0;
	}

	float currentX = (playerHP * startHPscaleX) / PlayerHP->GetMaxHP();
	float hpDiff = PlayerHP->GetMaxHP() - playerHP;

	float movePos = ((hpDiff / 2) * startHPscaleX) / PlayerHP->GetMaxHP();

	HPbar.lock()->m_transform->SetScale(glm::vec3(currentX, startHPscaleY, 1.0f));
	HPbar.lock()->m_transform->SetPosition(glm::vec3(startHPposX - movePos , HPbar.lock()->m_transform->GetPosition().y, 1.0f));
}

void GameController::updateStaminaUI()
{
	float playerSta = playerControl->GetStamina();

	if (playerSta < 0)
	{
		playerSta = 0;
	}

	float currentX = (playerSta * startStaminascaleX) / playerControl->GetMaxStamina();
	float staDiff = playerControl->GetMaxStamina() - playerSta;

	float movePos = ((staDiff / 2) * startStaminascaleX) / playerControl->GetMaxStamina();

	Staminabar.lock()->m_transform->SetScale(glm::vec3(currentX, startStaminascaleY, 1.0f));
	Staminabar.lock()->m_transform->SetPosition(glm::vec3(startStaminaposX - movePos, Staminabar.lock()->m_transform->GetPosition().y, 1.0f));
}

void GameController::AddPool(ObjectPool* pool, int type)
{
	Pools[type] = pool;
}

ObjectPool* GameController::GetPool(int type) {
	return Pools[type];
}

void GameController::updateSpawner() 
{
	if ((currScoreCheckpoint) < (Amplifiers.size())) {

		if (ScoreValue >= scoreCheckpoint[currScoreCheckpoint])
		{
			int randPreset = rand() % Presets.size();

			CurrAmplifier = Amplifiers[currScoreCheckpoint].get();
			CurrPreset = Presets[randPreset].get();

			for (EnemySpawner* spawner : Spawners) {
				spawner->SpawnAmplifier = CurrAmplifier;
				spawner->SpawnPreset = CurrPreset;
				spawner->updateSpawner();
			}

			currScoreCheckpoint += 1;

			ENGINE_INFO("update difficulty");
		}
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

void GameController::LoadData() {
	EquipmentManager* equipmentManager = m_gameObject->GetComponent<EquipmentManager>();
	if (equipmentManager != nullptr) {
		Data = std::make_unique<PlayerData>();
		Serialization::LoadObject(*Data.get(), DataPath);

		equipmentManager->SetWeaponUnlockData(Data->Weapons);
		equipmentManager->SetArtifactUnlockData(Data->Artifacts);
	}
	else {
		ENGINE_ERROR("No Equipment Mannager Found In GameController");
	}
}

void GameController::SaveData() {
	Serialization::SaveObject(*Data, DataPath);
}