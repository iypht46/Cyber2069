#include "Animator.hpp"

#include "Core/Logger.hpp"
#include <Serialization/Serialization.h>

Animator::Animator()
{
	timeElapse = 0;
	m_currentUVFrames = glm::vec2(0);

	Factory<Animator>::Add(this);
}

void Animator::Init() {
	m_controller = std::make_shared<AnimationController>();

	try
	{
		Serialization::LoadObject<AnimationController>(*m_controller, sr_controllerPath);
	}
	catch (const std::exception&)
	{
		ENGINE_ERROR("No animation controller path assigned");
	}


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
	setCurrentState(m_controller->GetState(state));
}

void Animator::setCurrentState(std::weak_ptr<AnimationState> state) {
	m_currentState = state;

	m_currentUVFrames = m_currentState.lock()->animation->getStartPosition();
}

void Animator::animUpdate(float dt)
{
	glm::vec2 start = m_currentState.lock()->animation->getStartPosition();
	glm::vec2 end = m_currentState.lock()->animation->getEndPosition();

	if (m_currentUVFrames.y < end.y || (m_currentUVFrames.y == end.y && m_currentUVFrames.x < end.x))
	{
		timeElapse += dt;
		if (timeElapse > 1.0f / (framePerSec * m_currentState.lock()->animation->getSpeedMultiplier()))
		{
			timeElapse = 0;
			++m_currentUVFrames.x;

			if (m_currentUVFrames.x > m_controller->getSheetSize().x) {
				++m_currentUVFrames.y;
			}
		}
	}
	else if (m_currentState.lock()->loop) {
		m_currentUVFrames = start;
	}
	else if (!m_currentState.lock()->nextState.expired()) {
		setCurrentState(m_currentState.lock()->nextState);
	}
}

void Animator::setFramePerSec(float frame) {
	framePerSec = frame;
}

Animator::~Animator()
{
}
