#pragma once
//Standard Headers
#include <unordered_map>
#include <glm/glm.hpp>

//Headers
#include "Component.hpp"

class Animation
{
protected:
	glm::vec2 m_StartPosition;
	glm::vec2 m_EndPosition;
	bool m_looping;
	std::string stateName;
	int speedMultiplier;
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
};

