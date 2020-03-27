#include "AnimationController.hpp"

AnimationState::AnimationState() {

}

AnimationState::AnimationState(std::shared_ptr<Animation> anim) {
	animation = anim;
}

AnimationController::AnimationController()
{
	m_spriteSheetFrameSize = glm::vec2(0.0f, 0.0f);

}

void AnimationController::AddState(std::shared_ptr<AnimationState> state) {
	m_states.push_back(state);
}

void AnimationController::AddState(std::shared_ptr<Animation> anim) {
	std::shared_ptr<AnimationState> state = std::make_shared<AnimationState>(anim);
	m_states.push_back(state);
}

void AnimationController::AddState(std::shared_ptr<Animation> anim, bool isloop) {
	std::shared_ptr<AnimationState> state = std::make_shared<AnimationState>(anim);
	state->loop = isloop;
	m_states.push_back(state);
}

void AnimationController::setSheetSize(glm::vec2 frameSize) {
	m_spriteSheetFrameSize = frameSize;
}

glm::vec2 AnimationController::getSheetSize() {
	return m_spriteSheetFrameSize;
}

std::shared_ptr<AnimationState> AnimationController::GetState(int state)
{
	return m_states.at(state);
}

AnimationController::~AnimationController()
{
}
