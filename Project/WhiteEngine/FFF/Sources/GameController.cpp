#include "GameController.hpp"

GameController* GameController::instance = nullptr;

GameController::GameController() {
	if (instance == nullptr) 
	{
		instance = this;

		FlyerSpawner = new GameObject();
		BomberSpawner = new GameObject();
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

}

void GameController::OnEnable() {

}

void GameController::OnStart() {

	FlyerSpawner->AddComponent<EnemySpawner>();
	FlyerSpawner->GetComponent<EnemySpawner>()->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);
	FlyerSpawner->GetComponent<EnemySpawner>()->SetSpawnType(POOL_TYPE::ENEMY_FLYER);

	BomberSpawner->AddComponent<EnemySpawner>();
	BomberSpawner->GetComponent<EnemySpawner>()->SetSpawnRange(Graphic::Window::GetWidth() / 2, Graphic::Window::GetHeight() / 2, Graphic::Window::GetWidth() / -2, Graphic::Window::GetHeight() / -2);
	BomberSpawner->GetComponent<EnemySpawner>()->SetSpawnType(POOL_TYPE::ENEMY_BOMBER);

	Spawner.push_back(FlyerSpawner->GetComponent<EnemySpawner>());
	Spawner.push_back(BomberSpawner->GetComponent<EnemySpawner>());

	EnemyAmplifier* a1 = new EnemyAmplifier;
	a1->EnemySpawnRate = 5.0f;
	Amplifier.push_back(a1);

	EnemyAmplifier* a2 = new EnemyAmplifier;
	a2->EnemySpawnRate = 0.5f;
	Amplifier.push_back(a2);

	EnemyPreset* p1 = new EnemyPreset;
	p1->BomberRatio = 0.5f;
	p1->FlyerRatio = 1.0f;
	Preset.push_back(p1);

}

void GameController::OnUpdate(float dt) 
{
	int sc = ScoreValue;
	this->ScoreText->GetComponent<TextRenderer>()->SetText("Score: " + to_string(sc));

	updateHPui();
	updateStaminaUI();
	updateSpawner();
}

void GameController::OnFixedUpdate(float dt) {

}

void GameController::OnDisable() {

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
	startHPscaleX = hpbar->m_transform.GetScale().x;
	startHPscaleY = hpbar->m_transform.GetScale().y;
	startHPposX = hpbar->m_transform.GetPosition().x;
}

void GameController::AssignStaminabar(GameObject* staminabar)
{
	this->Staminabar = staminabar;
	startStaminascaleX = staminabar->m_transform.GetScale().x;
	startStaminascaleY = staminabar->m_transform.GetScale().y;
	startStaminaposX = staminabar->m_transform.GetPosition().x;
}

void GameController::AssignPlayer(GameObject* player) {
	this->PlayerHP = player->GetComponent<HPsystem>();
	this->player = player->GetComponent<PlayerController>();
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

	HPbar->m_transform.SetScale(glm::vec3(currentX, startHPscaleY, 1.0f));
	HPbar->m_transform.SetPosition(glm::vec3(startHPposX - movePos, HPbar->m_transform.GetPosition().y, 1.0f));


}

void GameController::updateStaminaUI()
{
	float playerSta = player->GetStamina();

	if (playerSta < 0)
	{
		playerSta = 0;
	}

	float currentX = (playerSta * startStaminascaleX) / player->GetMaxStamina();
	float staDiff = player->GetMaxStamina() - playerSta;

	float movePos = ((staDiff / 2) * startStaminascaleX) / player->GetMaxStamina();

	Staminabar->m_transform.SetScale(glm::vec3(currentX, startStaminascaleY, 1.0f));
	Staminabar->m_transform.SetPosition(glm::vec3(startStaminaposX - movePos, Staminabar->m_transform.GetPosition().y, 1.0f));
}

void GameController::AddPool(ObjectPool* pool, int type)
{
	Pools.insert(pair<int, ObjectPool*>(type, pool));
}

ObjectPool* GameController::GetPool(int type) {
	return Pools[type];
}

void GameController::updateSpawner() 
{
	if ((currScoreCheckpoint) < (Amplifier.size())) {

		if (ScoreValue >= scoreCheckpoint[currScoreCheckpoint])
		{
			int randPreset = rand() % Preset.size();

			for (EnemySpawner* es : Spawner) {
				switch (es->GetType())
				{
				case POOL_TYPE::ENEMY_FLYER:
					es->SetSpawnRate((Amplifier[currScoreCheckpoint]->EnemySpawnRate) / Preset[randPreset]->FlyerRatio);
					//es->SetSpawnEnemyBasicStats(Amplifier[currScoreCheckpoint]->FlyerSpeed, Amplifier[currScoreCheckpoint]->FlyerHP, Amplifier[currScoreCheckpoint]->FlyerDmg);
					
					break;
				case POOL_TYPE::ENEMY_BOMBER:
					es->SetSpawnRate((Amplifier[currScoreCheckpoint]->EnemySpawnRate) / Preset[randPreset]->BomberRatio);
					//es->SetSpawnEnemyBasicStats(Amplifier[currScoreCheckpoint]->BomberSpeed, Amplifier[currScoreCheckpoint]->BomberHP, Amplifier[currScoreCheckpoint]->BomberDmg);
					//es->SetSpawnEnemyDashStats(Amplifier[currScoreCheckpoint]->BomberAimTime, Amplifier[currScoreCheckpoint]->BomberDashSpeed);
					//es->SetSpawnEnemyExplodeStats(Amplifier[currScoreCheckpoint]->BomberExplodeDMG, Amplifier[currScoreCheckpoint]->BomberExplodeRadius);
					break;
				default:
					break;
				}
			}

			currScoreCheckpoint += 1;
		}
	}
}