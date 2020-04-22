#pragma once
#include "Equipment.hpp"

class Artifact : public Equipment 
{
public:
	virtual void Modify(GameObject* obj) = 0;
	virtual void GameTimeBehaviour(float dt) = 0;
};