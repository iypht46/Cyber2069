#include "Explosion.hpp"



Explosion::Explosion()
{

}

void Explosion::OnAwake() {
	thisTransform = m_gameObject->m_transform.get();
	ps = Physic::PhysicScene::GetInstance();
	targetlayer = ps->GetLayerFromString(targetLayer);
}

void Explosion::Explode() {
	Physic::Colliders colliders = ps->GetColliderLayer(targetlayer);

	for (Collider* c : colliders) {
		float distance = glm::length(c->GetGameObject()->m_transform->GetPosition() - thisTransform->GetPosition());
		if (distance <= m_radius) {
			HPsystem* hp = c->GetGameObject()->GetComponent<HPsystem>();
			if (hp != nullptr) {
				hp->TakeDamage(m_damage);
			}
		}
	}
}

void Explosion::SetLayer(std::string layer) {
	targetLayer = layer;
	targetlayer = ps->GetLayerFromString(layer);
}

void Explosion::SetDamage(float val) {
	this->m_damage = val;
}

void Explosion::SetRadius(float val) {
	this->m_radius = val;
}

Explosion::~Explosion()
{
}