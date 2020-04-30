#include "Animator.hpp"

#include "Core/GameInfo.h"
#include "Core/Logger.hpp"
#include <Serialization/Serialization.h>

Animator::Animator()
{
	timeElapse = 0;
	m_currentUVFrames = glm::vec2(0);

	Factory<Component, Animator>::Add(this);
}

void Animator::Init() {
	if (m_controller == nullptr) {
		m_controller = std::make_shared<AnimationController>();
	}
	else {
		ENGINE_WARN("Already have animation controller, make sure you didn't Initialize animator component twice.");
	}

	try
	{
		Serialization::LoadObject(*m_controller, sr_controllerPath);
	}
	catch (const std::exception&)
	{
		ENGINE_WARN("No animation controller path assigned");
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

void Animator::setNextState(int state) {
	setNextState(m_controller->GetState(state));
}

void Animator::setNextState(std::weak_ptr<AnimationState> state) {
	if (!state.expired()) {
		m_nextState = state;
	}
	else {
		ENGINE_WARN("State invalid");
	}
}

void Animator::setCurrentState(int state) {
	setCurrentState(m_controller->GetState(state));
}

void Animator::setCurrentState(std::weak_ptr<AnimationState> state) {
	if (!state.expired()) {
		m_currentState = state;

		m_currentUVFrames = m_currentState.lock()->animation->getStartPosition();
	}
	else {
		ENGINE_WARN("Assigned state invalid");
	}
}

void Animator::animUpdate(float dt)
{
	if (!m_currentState.expired()) {
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
		//if loop, start the animation over
		else if (m_currentState.lock()->loop) {
			m_currentUVFrames = start;
		}
		//if next state is assigned, switch to that state
		else if (!m_nextState.expired()) {
			setCurrentState(m_nextState);
			m_nextState.reset();
		}
		//if next state is not assigned, switch to default next state from AnimationState
		else if (!m_currentState.lock()->nextState.expired()) {
			setCurrentState(m_currentState.lock()->nextState);
		}
		//else, no animation will be played next
		else {
			m_currentState.reset();
		}
	}
}

void Animator::setFramePerSec(float frame) {
	framePerSec = frame;
}

Animator::~Animator()
{
	Factory<Component, Animator>::Remove(this);
}
