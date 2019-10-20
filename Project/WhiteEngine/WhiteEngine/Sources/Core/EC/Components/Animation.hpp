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
public:
	Animation();
	glm::vec2 getStartPosition();
	glm::vec2 getEndPosition();
	void setState(std::string s);
	void setStartPosition(int startPosX,int startPosY);
	void setEndPosition(int endPosX, int endPosY);
	bool isLooping();
	~Animation();
};

