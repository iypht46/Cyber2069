#include "Animator.hpp"

#include <Serialization/Serialization.h>

Animator::Animator()
{
	timeElapse = 0;
	m_currentUVFrames = glm::vec2(0);
	Factory<Animator>::Add(this);
}

void Animator::Init() {
	m_controller = std::make_shared<AnimationController>();

	//Serialization::LoadObject<AnimationController>(*m_controller, sr_controllerPath);

	m_currentState = m_controller->m_defaultState;
}

void Animator::AssignController(std::shared_ptr <AnimationController> animControl) {
	m_controller = animControl;
}

glm::vec2 Animator::GetCurrentUVFrame() {
	glm::vec2 tmp(m_currentUVFrames.x / m_controller->getSheetSize().x, m_currentUVFrames.y / m_controller->getSheetSize().y);

	return tmp;
}

void Animator::setCurrentState(int state) {
	m_currentState = m_controller->GetState(state);

	m_currentUVFrames = m_currentState->getStartPosition();
}

void Animator::animUpdate(float dt)
{
	if (m_currentUVFrames.x < m_currentState->getEndPosition().x)
	{
		timeElapse += dt;
		if (timeElapse > 1.0f / (framePerSec * m_currentState->getSpeedMultiplier()))
		{
			timeElapse = 0;
			m_currentUVFrames.x++;
		}
	}
	else if (m_currentState->isLooping()) {
		m_currentUVFrames.x = m_currentState->getStartPosition().x;
	}
}

void Animator::setFramePerSec(float frame) {
	framePerSec = frame;
}

Animator::~Animator()
{
}
