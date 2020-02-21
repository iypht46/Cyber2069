#include "Explosion.hpp"



Explosion::Explosion()
{
	m_damage = 10.0f;
	m_radius = 500.0f;
	bomber = &(m_gameObject)->m_transform;
}


Explosion::~Explosion()
{
}

void Explosion::Explode() {
	
}

void Explosion::OnAwake() {

}

void Explosion::OnEnable() {

}

void Explosion::OnStart() {

}

void Explosion::OnUpdate(float dt) {

}

void Explosion::OnFixedUpdate(float dt) {

}

void Explosion::OnDisable() {

}
