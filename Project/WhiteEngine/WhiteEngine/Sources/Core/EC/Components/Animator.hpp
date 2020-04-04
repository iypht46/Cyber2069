#pragma once

#include "Core/Animator/AnimationController.hpp"
//#include "../../Animator/AnimationController.hpp"

#include "../../Animation/Animation.hpp"
#include "../../Animation/AnimationController.hpp"

#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

namespace Tools { class AnimatorEC; }

class Animator : public Component
{
	friend class Tools::AnimatorEC;
private:
	//serialized data
	std::string sr_controllerPath;
	float framePerSec = 12;


	//runtime var
	float timeElapse;
	std::shared_ptr<AnimationController> m_controller;
	std::weak_ptr<AnimationState> m_currentState;
	glm::vec2 m_currentUVFrames;

public:
	Animator();

	virtual void Init();
	void AssignController(std::shared_ptr <AnimationController> animControl);

	void setCurrentState(int state);
	void setCurrentState(std::weak_ptr <AnimationState> state);

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
