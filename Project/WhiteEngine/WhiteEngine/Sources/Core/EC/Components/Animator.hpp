#pragma once
//Standard Headers
#include <unordered_map>

//Headers
#include "Component.hpp"
class Animator
{
protected:
	int currFrame;
	int numFrame;
	int spriteSheet[5][7] = 
	{
		{1, 2, 3, 4, 5, 6, 7} ,
		{8, 9, 10, 11, 12, 0, 0} ,
		{13, 14, 15, 16, 17, 18, 0} ,
		{19, 20, 21, 22, 23, 24, 0} ,
		{25, 26, 27, 0, 0, 0, 0} ,
	};
	int offSetY;
	std::unordered_map<std::string, std::string> animState;

	int tmpDeley;
public:
	Animator();
	void animUpdate();
	void printSpriteSheet();
	void setAnimFrame(std::string s);
	int getCurrFrame();
	int getNumFrame();
	int getOffSetY();
	~Animator();
};

