#pragma once
//Standard Headers
#include <unordered_map>
#include <glm/glm.hpp>

#include <Serialization/glmCereal.h>
#include <cereal/types/string.hpp>

class Animation
{
public:
	std::string stateName;
	glm::vec2 m_StartPosition;
	glm::vec2 m_EndPosition;
	int speedMultiplier = 1;


	Animation();
	glm::vec2 getStartPosition();
	glm::vec2 getEndPosition();
	int getSpeedMultiplier();
	void setState(std::string s);
	void setStartPosition(int startPosX,int startPosY);
	void setEndPosition(int endPosX, int endPosY);
	void setSpeedMultiplier(int multiplier);
	~Animation();

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			stateName,
			m_StartPosition,
			m_EndPosition,
			speedMultiplier
		);
	}
};
