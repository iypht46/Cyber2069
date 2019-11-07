#include "Animation.hpp"



Animation::Animation()
{
	m_StartPosition = glm::vec2(0.0f, 0.0f);
	m_EndPosition = glm::vec2(0.0f, 0.0f);
	m_looping = false;
	speedMultiplier = 0;

}

glm::vec2 Animation::getStartPosition() {
	return m_StartPosition;
}

glm::vec2 Animation::getEndPosition() {
	return m_EndPosition;
}

int Animation::getSpeedMultiplier() {
	return speedMultiplier;
}

void Animation::setState(std::string s) {
	stateName = s;
}

void Animation::setStartPosition(int startPosX, int startPosY) {
	m_StartPosition = glm::vec2(startPosX, startPosY);
}

void Animation::setEndPosition(int endPosX, int endPosY) {
	m_EndPosition = glm::vec2(endPosX, endPosY);
}

void Animation::setSpeedMultiplier(int multiplier) {
	speedMultiplier = multiplier;
}

bool Animation::isLooping() {
	return m_looping;
}

Animation::~Animation()
{
}
