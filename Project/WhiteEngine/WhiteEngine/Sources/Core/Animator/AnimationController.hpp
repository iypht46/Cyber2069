#pragma once


#include "Core/EC/Components/Animation.hpp"

class AnimationController
{
protected:
	std::vector<Animation*> m_states;
	glm::vec2 m_spriteSheetFrameSize;
public:
	AnimationController();
	void AddState(Animation* anim);
	Animation* GetState(int state);
	void setSheetSize(glm::vec2 frameSize);
	
	glm::vec2 getSheetSize();
	~AnimationController();
};

