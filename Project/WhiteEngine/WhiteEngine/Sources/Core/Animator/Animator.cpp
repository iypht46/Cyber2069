#include "Animator.hpp"



Animator::Animator()
{
	tmpDelay = 0;
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

void Animator::animUpdate() 
{
	if (m_currentUVFrames.x < m_currentState->getEndPosition().x)
	{
		tmpDelay++;
		if (tmpDelay == 5) 
		{
			tmpDelay = 0;
			m_currentUVFrames.x++;
		}
	}
	else {
		m_currentUVFrames.x = m_currentState->getStartPosition().x;
	}
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
