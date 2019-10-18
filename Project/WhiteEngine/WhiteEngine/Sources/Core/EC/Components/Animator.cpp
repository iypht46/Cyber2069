#include "Animator.hpp"
#include <iostream>


Animator::Animator()
{
	currFrame = 0;
	numFrame = 0;
	offSetY = 0;

	tmpDeley = 0;

	animState["Idle"] = "Idle";
	animState["Running"] = "Running";
	animState["Die"] = "Die";
	animState["Jumping"] = "Jumping";
	animState["Dash"] = "Dash";

}


void Animator::animUpdate() {
	
	if (currFrame < numFrame - 1) {
		/*for (int i = currFrame; i < numFrame; i++) {
			std::cout << spriteSheet[offSetY][i] << "\t";
			currFrame++;
		}*/
		tmpDeley++;

		if (tmpDeley == 10)
		{
			tmpDeley = 0;
			currFrame += 1;
			std::cout << currFrame << "\t";
		}
	}
	else 
	{
		currFrame = 0;
		std::cout << currFrame << "\t";
	}

}

void Animator::printSpriteSheet() {
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 7; j++) {
			std::cout << spriteSheet[i][j] << "\t";
		}
		std::cout << std::endl;
	}

}

void Animator::setAnimFrame(std::string s) {
	if (animState.at("Idle") == s) {
		offSetY = 0;
		numFrame = 7;
	}
	else if (animState.at("Running") == s) {
		offSetY = 1;
		numFrame = 5;
	}
	else if (animState.at("Die") == s) {
		offSetY = 2;
		numFrame = 7;
	}
	else if (animState.at("Jumping") == s) {
		offSetY = 3;
		numFrame = 4;
	}
	else if (animState.at("Dash") == s) {
		offSetY = 4;
		numFrame = 3;
	}
}

int Animator::getCurrFrame() {
	return currFrame;
}

int Animator::getNumFrame() {
	return numFrame;
}


int Animator::getOffSetY() {
	return offSetY;
}

Animator::~Animator()
{
}
