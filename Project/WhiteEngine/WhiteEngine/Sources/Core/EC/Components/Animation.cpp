#include "Animation.hpp"



Animation::Animation()
{
	m_StartPosition = glm::vec2(0.0f, 0.0f);
	m_EndPosition = glm::vec2(0.0f, 0.0f);
	m_looping = false;

}

glm::vec2 Animation::getStartPosition() {
	return m_StartPosition;
}

glm::vec2 Animation::getEndPosition() {
	return m_EndPosition;
}

void Animation::setState(std::string s) {
	stateName = s;
}

bool Animation::isLooping() {
	return m_looping;
}

Animation::~Animation()
{
}
