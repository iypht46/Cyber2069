#pragma once
//White Engine
#include "Core/Animation/Animation.hpp"
#include "Core/Animation/AnimationController.hpp"
//Third Party
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

namespace Tools { class AnimatorEC; }

class Animator : public Component
{
	friend class Tools::AnimatorEC;
private:
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
	bool SetControllerPath(std::string path);
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
