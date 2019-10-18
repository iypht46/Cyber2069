#include "AnimationController.hpp"



AnimationController::AnimationController()
{
	m_spriteSheetFrameSize = glm::vec2(0.0f, 0.0f);

}

void AnimationController::AddState(Animation* anim) {
	m_states.push_back(anim);
}

void AnimationController::setSheetSize(glm::vec2 frameSize) {
	m_spriteSheetFrameSize = frameSize;
}

glm::vec2 AnimationController::getSheetSize() {
	return m_spriteSheetFrameSize;
}

Animation* AnimationController::GetState(int state) 
{
	return m_states.at(state);
}

AnimationController::~AnimationController()
{
}
