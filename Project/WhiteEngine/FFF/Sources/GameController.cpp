#include "GameController.hpp"

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
	startHPscaleX = HPbar.lock()->m_transform->GetScale().x;
	startHPscaleY = HPbar.lock()->m_transform->GetScale().y;
	startHPposX = HPbar.lock()->m_transform->GetPosition().x;

	startStaminascaleX = Staminabar.lock()->m_transform->GetScale().x;
	startStaminascaleY = Staminabar.lock()->m_transform->GetScale().y;
	startStaminaposX = Staminabar.lock()->m_transform->GetPosition().x;
}

void GameController::OnStart() {
	this->PlayerHP = player.lock()->GetComponent<HPsystem>();
	this->playerControl = player.lock()->GetComponent<PlayerController>();

	ENGINE_INFO("GameControl Creating Bullets");
	ENGINE_INFO("======================================================================");
	CreatePool(PrefabPath("Bullet_MG"), POOL_TYPE::BULLET_MG, 20);
	CreatePool(PrefabPath("Bullet_GL"), POOL_TYPE::BULLET_GL, 20);
	CreatePool(PrefabPath("Bullet_ZP"), POOL_TYPE::BULLET_ZP, 20);
	CreatePool(PrefabPath("Bullet_BH"), POOL_TYPE::BULLET_BH, 20);
	CreatePool(PrefabPath("Bullet_Fume"), POOL_TYPE::BULLET_FUME, 20);

	ENGINE_INFO("GameControl Creating Enemy");
	ENGINE_INFO("======================================================================");

	//flyer spawner
	ENGINE_INFO("GameControl Creating Flyer");
	CreatePool(PrefabPath("Flyer"), POOL_TYPE::ENEMY_FLYER, 50);
	CreateSpawner(POOL_TYPE::ENEMY_FLYER)->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);

	//bomber spawner
	ENGINE_INFO("GameControl Creating Bomber");
	CreatePool(PrefabPath("Bomber"), POOL_TYPE::ENEMY_BOMBER, 50);
	CreateSpawner(POOL_TYPE::ENEMY_BOMBER)->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);

	//Tank
	ENGINE_INFO("GameControl Creating Tank");
	CreatePool(PrefabPath("Tank"), POOL_TYPE::ENEMY_TANK, 10);
	CreateSpawner(POOL_TYPE::ENEMY_TANK)->SetSpawnRange(0, 100, 0, 100);

	//Charger
	ENGINE_INFO("GameControl Creating Charger");
	CreatePool(PrefabPath("Charger"), POOL_TYPE::ENEMY_CHARGER, 10);
	CreateSpawner(POOL_TYPE::ENEMY_CHARGER)->SetSpawnRange(0, 100, 0, 100);

	//Spitter
	ENGINE_INFO("GameControl Creating Spitter");
	CreatePool(PrefabPath("Spitter"), POOL_TYPE::ENEMY_SPITTER, 10);
	CreateSpawner(POOL_TYPE::ENEMY_SPITTER)->SetSpawnRange(-400, 100, 400, 100);

	//Queen spawner
	ENGINE_INFO("GameControl Creating Queen");
	CreatePool(PrefabPath("Queen"), POOL_TYPE::ENEMY_QUEEN, 1);
	QueenSpawner = CreateSpawner(POOL_TYPE::ENEMY_QUEEN);
	QueenSpawner->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);

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
	a->EnemySpawnRate = 5.0f;
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
	int sc = ScoreValue;
	this->ScoreText.lock()->GetComponent<TextRenderer>()->SetText("Score: " + to_string(sc));

	updateHPui();
	updateStaminaUI();
	updateSpawner();

	//spawn enemy
	for (EnemySpawner* spawner : Spawners) {
		spawner->OnUpdate(dt);
	}
}

void GameController::SpawnQueen() {
	if (QueenSpawner != nullptr) {
		QueenSpawner->SpawnEnemy();
	}
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