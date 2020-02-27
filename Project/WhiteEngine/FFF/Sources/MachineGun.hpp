#pragma once
#include "Weapon.hpp"

class MachineGun : public Weapon {
public:
	MachineGun();
	void Modify(GameObject* obj);
	void GameTimeBehaviour(float dt);
};
