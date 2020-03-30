#include "Explosion.hpp"



Explosion::Explosion()
{

}

void Explosion::Init() {
	m_damage = 100.0f;
	m_radius = 500.0f;

	thisTransform = &(m_gameObject->m_transform);
	ps = Physic::PhysicScene::GetInstance();
	targetLayer = ps->GetLayerFromString("Player");
}

Explosion::~Explosion()
{
}

void Explosion::Explode() {
	Physic::Colliders colliders = ps->GetColliderLayer(targetLayer);
	for (Collider* c : colliders) {

		float distance = glm::length(c->GetGameObject()->m_transform.GetPosition() -
			thisTransform->GetPosition());
		if (distance <= m_radius) {
			HPsystem* hp = c->GetGameObject()->GetComponent<HPsystem>();
			if (hp != nullptr) {
				hp->TakeDamage(m_damage);
			}
		}
	}
}

void Explosion::SetLayer(std::string layer) {
	targetLayer = ps->GetLayerFromString(layer);
}

void Explosion::SetDamage(float val) {
	this->m_damage = val;
}

void Explosion::SetRadius(float val) {
	this->m_radius = val;
}

void Explosion::OnStart() {

}

void Explosion::OnUpdate(float dt) {

}

void Explosion::OnFixedUpdate(float dt) {

}
