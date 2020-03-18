#pragma once


#include "../../Animation/Animation.hpp"
#include "../../Animation/AnimationController.hpp"

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

class Animator : public Component
{
private:
	/*undetermined*/AnimationController* m_controller;
	float framePerSec;

	float timeElapse;

	std::shared_ptr<Animation> m_currentState;
	glm::vec2 m_currentUVFrames;

public:
	Animator();
	void AssignController(AnimationController* animControl);
	
	void setCurrentState(int state);

	void animUpdate(float dt);

	void setFramePerSec(float frame);

	glm::vec2 GetCurrentUVFrame();
	~Animator();

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDestroy();

//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			framePerSec
		);
	}
};

CEREAL_REGISTER_TYPE(Animator);