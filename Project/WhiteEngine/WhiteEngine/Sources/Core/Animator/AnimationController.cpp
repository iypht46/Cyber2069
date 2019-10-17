#include "AnimationController.hpp"



AnimationController::AnimationController()
{
	m_spriteSheetFrameSize = glm::vec2(0.0f, 0.0f);

}

void AnimationController::AddState(Animation* anim) {
	//m_states.push_back(anim);
}

void AnimationController::setSheetSize(glm::vec2 frameSize) {
	m_spriteSheetFrameSize = frameSize;
}

AnimationController::~AnimationController()
{
}
