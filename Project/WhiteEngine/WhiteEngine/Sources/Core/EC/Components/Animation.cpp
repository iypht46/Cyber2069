#include "Animation.hpp"



Animation::Animation()
{
	currFrame = 0;
	numFrame = 0;
	offSetY = 0;
	animState["Idle"] = "Idle";
	animState["Running"] = "Running";
	animState["Die"] = "Die";
	animState["Jumping"] = "Jumping";
	animState["Dash"] = "Dash";

}


void Animation::animUpdate() {
	if (currFrame < numFrame) {
		for (int i = currFrame; i < numFrame; i++) {
			std::cout << spriteSheet[offSetY][i] << "\t";
			currFrame++;
		}
	}
	else {
		std::cout << std::endl;
		currFrame = 0;
	}

}

void Animation::printSpriteSheet() {
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 7; j++) {
			std::cout << spriteSheet[i][j] << "\t";
		}
		std::cout << std::endl;
	}

}

void Animation::setAnimFrame(std::string s) {
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
		numFrame = 6;
	}
	else if (animState.at("Jumping") == s) {
		offSetY = 3;
		numFrame = 6;
	}
	else if (animState.at("Dash") == s) {
		offSetY = 4;
		numFrame = 3;
	}
}

int Animation::getCurrFrame() {
	return currFrame;
}

int Animation::getNumFrame() {
	return numFrame;
}


int Animation::getOffSetY() {
	return offSetY;
}

Animation::~Animation()
{
}
