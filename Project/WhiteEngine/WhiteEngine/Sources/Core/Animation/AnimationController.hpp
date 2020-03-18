#pragma once

#include "Animation.hpp"
#include <memory>

#include <cereal/types/vector.hpp>

class AnimationController
{
protected:
	std::vector<std::shared_ptr<Animation>> m_states;
	glm::vec2 m_spriteSheetFrameSize;
public:
	AnimationController();
	void AddState(std::shared_ptr<Animation> anim);
	std::shared_ptr<Animation> GetState(int state);
	void setSheetSize(glm::vec2 frameSize);
	
	glm::vec2 getSheetSize();
	~AnimationController();

//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive( 
			m_states,
			m_spriteSheetFrameSize
		);
	}
};

