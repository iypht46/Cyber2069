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

	EnemySpawner* FlyerSpawner = m_gameObject->AddComponent<EnemySpawner>();
	FlyerSpawner->EnemyTarget = player.lock().get();
	//FlyerSpawn()->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);
	FlyerSpawner->SetSpawnRange(0, 100, 0, 100);
	FlyerSpawner->SetSpawnType(POOL_TYPE::ENEMY_CHARGER);

	EnemySpawner* BomberSpawner = m_gameObject->AddComponent<EnemySpawner>();
	BomberSpawner->EnemyTarget = player.lock().get();
	BomberSpawner->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);
	BomberSpawner->SetSpawnType(POOL_TYPE::ENEMY_BOMBER);

	Spawners.push_back(FlyerSpawner);
	Spawners.push_back(BomberSpawner);


	//difficulty ting needs to go out side---------------------------
	shared_ptr<EnemyAmplifier> a = std::make_shared<EnemyAmplifier>();
	a->EnemySpawnRate = 1.0f;
	a->FlyerHP = 1.0f;
	a->FlyerSpeed = 100.0f;
	a->FlyerDmg = 10.0f;

	a->BomberHP = 1.0f;
	a->BomberSpeed = 300.0f;
	a->BomberDmg = 10.0f;
	a->BomberAimTime = 1.0f;
	a->BomberDashSpeed = 700.0f;
	a->BomberExplodeDMG = 30.0f;
	a->BomberExplodeRadius = 200.0f;

	Amplifiers.push_back(a);

	a = std::make_shared<EnemyAmplifier>();
	a->EnemySpawnRate = 0.5f;
	a->FlyerHP = 1.0f;
	a->FlyerSpeed = 300.0f;
	a->FlyerDmg = 10.0f;

	a->BomberHP = 1.0f;
	a->BomberSpeed = 300.0f;
	a->BomberDmg = 10.0f;
	a->BomberAimTime = 1.0f;
	a->BomberDashSpeed = 700.0f;
	a->BomberExplodeDMG = 30.0f;
	a->BomberExplodeRadius = 200.0f;

	Amplifiers.push_back(a);

	std::shared_ptr<EnemyPreset> p1 = std::make_shared<EnemyPreset>();
	p1->BomberRatio = 0.5f;
	p1->FlyerRatio = 1.0f;
	Presets.push_back(p1);
	//------------------------------------------------------------

	CurrAmplifier = Amplifiers[0].get();
	CurrPreset = Presets[0].get();

	SetActiveAllSpawner(false);
}

void GameController::OnUpdate(float dt)
{
	switch (CurrentState)
	{
	case GAME_STATE::MAINMENU:
		playerControl->GetGameObject()->SetActive(false);
		HPbar->SetActive(false);
		Staminabar->SetActive(false);
		ScoreText->SetActive(false);

		if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE)) 
		{
			CurrentState = GAME_STATE::GAMEPLAY;
		}


		break;
	case GAME_STATE::LOADOUT:
		break;
	case GAME_STATE::GAMEPLAY:
		playerControl->GetGameObject()->SetActive(true);
		HPbar->SetActive(true);
		Staminabar->SetActive(true);
		ScoreText->SetActive(true);

		this->ScoreText->GetComponent<TextRenderer>()->SetText("Score: " + to_string((int)ScoreValue));

		SetActiveAllSpawner(true);

		updateHPui();
		updateStaminaUI();
		updateSpawner();

		break;
	case GAME_STATE::ENDING:
		break;
	default:
		break;
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

void GameController::AssignScoreText(GameObject* ScoreText) {
	this->ScoreText = ScoreText;
}

void GameController::AssignHPbar(GameObject* hpbar) {
	this->HPbar = hpbar;
	startHPscaleX = hpbar->m_transform->GetScale().x;
	startHPscaleY = hpbar->m_transform->GetScale().y;
	startHPposX = hpbar->m_transform->GetPosition().x;
}

void GameController::AssignStaminabar(GameObject* staminabar)
{
	this->Staminabar = staminabar;
	startStaminascaleX = staminabar->m_transform->GetScale().x;
	startStaminascaleY = staminabar->m_transform->GetScale().y;
	startStaminaposX = staminabar->m_transform->GetPosition().x;
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

	HPbar->m_transform->SetScale(glm::vec3(currentX, startHPscaleY, 1.0f));
	HPbar->m_transform->SetPosition(glm::vec3(startHPposX - movePos , HPbar->m_transform->GetPosition().y, 1.0f));
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

	Staminabar->m_transform->SetScale(glm::vec3(currentX, startStaminascaleY, 1.0f));
	Staminabar->m_transform->SetPosition(glm::vec3(startStaminaposX - movePos, Staminabar->m_transform->GetPosition().y, 1.0f));
}

void GameController::AddPool(ObjectPool* pool, int type)
{
	Pools.insert(pair<int, ObjectPool*>(type, pool));
}

ObjectPool* GameController::GetPool(int type) {
	return Pools[type].get();
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

			ENGINE_INFO("updtae diff");
		}
	}
}

void GameController::AddSpawner(EnemySpawner* spawner) 
{
	Spawners.push_back(spawner);
}

void GameController::SetActiveAllSpawner(bool active) 
{
	for (EnemySpawner* e : Spawners) 
	{
		e->GetGameObject()->SetActive(active);
	}
}