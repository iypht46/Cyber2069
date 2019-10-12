#include "Animation.hpp"



Animation::Animation()
{
	currFrame = 0;
	numFrame = 0;
	offSetY = 0;
	animState["Idle"] = 0;
	animState["Running"] = 1;
	animState["Die"] = 2;
	animState["Jumping"] = 3;
	animState["Dash"] = 4;

}


void Animation::animUpdate() {
	
	
}

void Animation::printSpriteSheet() {
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 7; j++) {
			std::cout << spriteSheet[i][j] << "\t";
		}
		std::cout << std::endl;
	}

}

int Animation::setAnimFrame() {

}

int Animation::getCurrFrame() {
	return currFrame;
}

int Animation::getNumFrame() {
	return numFrame;
}

int Animation::getOffSetX() {
	return offSetX;
}

int Animation::getOffSetY() {
	return offSetY;
}

Animation::~Animation()
{
}
