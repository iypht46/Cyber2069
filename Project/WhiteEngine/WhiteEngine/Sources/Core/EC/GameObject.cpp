#include "GameObject.hpp"
#include "../Factory.h"

void GameObject::SetActive(bool activestate) {
	isActive = activestate;
}