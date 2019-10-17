#pragma once


#include "AnimationController.hpp"

class Animator : public Component
{
private:
	AnimationController m_controller;
	Animation m_currentState;
	glm::vec2 m_currentUVFrames;
public:
	Animator();
	void AssignController(AnimationController animControl);
	glm::vec2 GetCurrentUVFrame();
	~Animator();
};

