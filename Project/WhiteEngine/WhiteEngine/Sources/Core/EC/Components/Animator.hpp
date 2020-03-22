#pragma once


#include "../../Animation/Animation.hpp"
#include "../../Animation/AnimationController.hpp"

#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

class Animator : public Component
{
private:
	//serialized data
	std::string sr_controllerPath;
	float framePerSec = 12;

	std::shared_ptr <AnimationController> m_controller;

	//runtime var
	float timeElapse;
	std::shared_ptr<Animation> m_currentState;
	glm::vec2 m_currentUVFrames;

public:
	Animator();

	virtual void Init();
	void AssignController(std::shared_ptr <AnimationController> animControl);

	void setCurrentState(int state);

	void animUpdate(float dt);

	void setFramePerSec(float frame);

	glm::vec2 GetCurrentUVFrame();
	~Animator();

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			sr_controllerPath,
			framePerSec
		);
	}
};

CEREAL_REGISTER_TYPE(Animator);