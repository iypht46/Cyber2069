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
}

void GameController::OnFixedUpdate(float dt) {

}

void GameController::OnDisable() {

}