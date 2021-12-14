#include "../../Weapon.hpp"

Bullet::Bullet() {
	//shouldn't be here
	TargetLayers.insert("Enemy");
	TargetLayers.insert("GroundEnemy");
}

Bullet::~Bullet() {
}

bool Bullet::isTarget(std::string layer) {
	for (std::string target : TargetLayers) {
		if (layer == target) {
			return true;
		}
	}
	return false;
}

bool Bullet::isTarget(Physic::Layer layer) {
	for (std::string target : TargetLayers) {
		if (layer == Physic::PhysicScene::GetInstance()->GetLayerFromString(target)) {
			return true;
		}
	}
	return false;
}