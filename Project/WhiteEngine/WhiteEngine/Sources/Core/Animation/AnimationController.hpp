#pragma once

#include "Animation.hpp"
#include <memory>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

class AnimationController
{
protected:
	glm::vec2 m_spriteSheetFrameSize;
	std::vector<std::shared_ptr<Animation>> m_states;
public:
	std::shared_ptr<Animation> m_defaultState;

	AnimationController();
	void AddState(std::shared_ptr<Animation> anim);
	std::shared_ptr<Animation> GetState(int state);
	void setSheetSize(glm::vec2 frameSize);
	
	glm::vec2 getSheetSize();
	~AnimationController();

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			m_spriteSheetFrameSize,
			m_states,
			cereal::defer(m_defaultState)
		);

		//for default state
		archive.serializeDeferments();
	}
};

