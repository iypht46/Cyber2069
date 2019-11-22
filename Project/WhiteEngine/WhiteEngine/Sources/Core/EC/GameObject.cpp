#include "GameObject.hpp"
#include "../Factory.h"

void GameObject::SetActive(bool activestate) {
	isActive = activestate;
}

//return object active state
bool GameObject::Active() {
	return isActive;
}