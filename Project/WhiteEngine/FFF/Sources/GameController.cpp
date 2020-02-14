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

float GameController::GetScore(){
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

void GameController::AssignPlayer(GameObject* player) {
	this->PlayerHP = player->GetComponent<HPsystem>();
}

void GameController::updateHPui() {
	float currentX = (PlayerHP->GetHP() * startHPscaleX) / PlayerHP->GetMaxHP();
	float hpDiff = PlayerHP->GetMaxHP() - PlayerHP->GetHP();

	float movePos = ((hpDiff / 2) * startHPscaleX) / PlayerHP->GetMaxHP();

	HPbar->m_transform.SetScale(glm::vec3(currentX, startHPscaleY, 1.0f));
	HPbar->m_transform.SetPosition(glm::vec3(startHPposX - movePos , HPbar->m_transform.GetPosition().y, 1.0f));


}

void GameController::OnAwake() {

}

void GameController::OnEnable() {

}

void GameController::OnStart() {

}

void GameController::OnUpdate(float dt) 
{
	int sc = ScoreValue;
	this->ScoreText->GetComponent<TextRenderer>()->SetText("Score: " + to_string(sc));
	updateHPui();
}

void GameController::OnFixedUpdate(float dt) {

}

void GameController::OnDisable() {

}