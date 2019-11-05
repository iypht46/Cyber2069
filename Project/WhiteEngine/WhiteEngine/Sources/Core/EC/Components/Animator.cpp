#include "Animator.hpp"



Animator::Animator()
{
	timeElapse = 0;
	framePerSec = 12;
	m_currentUVFrames = glm::vec2(0);
}

void Animator::AssignController(AnimationController* animControl) {
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
		timeElapse+= dt;
		if (timeElapse > 1.0f/(framePerSec * m_currentState->getSpeedMultiplier())) 
		{
			timeElapse = 0;
			m_currentUVFrames.x++;
		}
	}
	else {
		m_currentUVFrames.x = m_currentState->getStartPosition().x;
	}
}

void Animator::setFramePerSec(float frame) {
	framePerSec = frame;
}

Animator::~Animator()
{
}

void Animator::OnAwake()
{
}

void Animator::OnEnable()
{
}

void Animator::OnStart()
{
}

void Animator::OnDisable()
{
}

void Animator::OnUpdate(float dt)
{
}

void Animator::OnFixedUpdate(float dt)
{
}

void Animator::OnDestroy()
{
}
