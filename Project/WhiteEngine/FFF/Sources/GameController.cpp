#include "GameController.hpp"

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

void GameController::OnAwake() {

}

void GameController::OnEnable() {

}

void GameController::OnStart() {

}

void GameController::OnUpdate(float dt) {

}

void GameController::OnFixedUpdate(float dt) {

}

void GameController::OnDisable() {

}