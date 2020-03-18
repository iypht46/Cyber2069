#pragma once
//Standard Headers
#include <unordered_map>
#include <glm/glm.hpp>

//Headers
#include "../EC/Components/Component.hpp"

#include <cereal/types/string.hpp>

class Animation
{
protected:
	std::string stateName;
	glm::vec2 m_StartPosition;
	glm::vec2 m_EndPosition;
	int speedMultiplier;
	bool m_looping;

public:
	Animation();
	glm::vec2 getStartPosition();
	glm::vec2 getEndPosition();
	int getSpeedMultiplier();
	void setState(std::string s);
	void setStartPosition(int startPosX,int startPosY);
	void setEndPosition(int endPosX, int endPosY);
	void setSpeedMultiplier(int multiplier);
	void setLooping(bool loop);
	bool isLooping();
	~Animation();

//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive( 
			stateName,
			m_StartPosition,
			m_EndPosition,
			speedMultiplier,
			m_looping
		);
	}
};