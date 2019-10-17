#include "Animator.hpp"



Animator::Animator()
{

}

void Animator::AssignController(AnimationController animControl) {
	m_controller = animControl;
}

glm::vec2 Animator::GetCurrentUVFrame() {
	return m_currentUVFrames;
}


Animator::~Animator()
{
}
