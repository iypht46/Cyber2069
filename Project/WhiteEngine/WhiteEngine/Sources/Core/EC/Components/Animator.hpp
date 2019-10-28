#pragma once


#include "../../Animator/AnimationController.hpp"

class Animator : public Component
{
private:
	AnimationController* m_controller;
	Animation* m_currentState;
	glm::vec2 m_currentUVFrames;

	int tmpDelay;

public:
	Animator();
	void AssignController(AnimationController* animControl);
	
	void setCurrentState(int state);

	void animUpdate();

	glm::vec2 GetCurrentUVFrame();
	~Animator();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDestroy();
};

