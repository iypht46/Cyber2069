#include "GroundPatrol.hpp"
#include "Graphic/GLRenderer.h"


GroundPatrol::GroundPatrol()
{
}


GroundPatrol::~GroundPatrol()
{
}

void GroundPatrol::Init() {
	m_stoppingDistance = 100.0f;
	m_speed = 150.0f;
	thisTransform = &(m_gameObject->m_transform);
	rb = m_gameObject->GetComponent<Rigidbody>();
	ps = Physic::PhysicScene::GetInstance();
}

void GroundPatrol::SetStopDis(float val) {
	this->m_stoppingDistance = val;
}

void GroundPatrol::SetSpeed(float val) {
	this->m_speed = val;
}

void GroundPatrol::Patrol() {
	Collider* Ground = nullptr;
	float posX, posY;

	posX = thisTransform->GetPosition().x;
	posY = thisTransform->GetPosition().y;
	float sign = posX / glm::abs(posX);

	Ground = ps->Raycast(Physic::Ray(posX, posY, posX + (sign*m_stoppingDistance), posY - (thisTransform->GetScale().y / 2)), ps->GetLayerFromString("Platform"));

	if (Ground->GetCollisionState() == COL_STATE::NONE) {
		if (rb->GetVelocity().x < 0) {
			rb->SetVelocity(glm::vec3(50.0f, 0, 0));
		}
		else {
			rb->SetVelocity(glm::vec3(-50.0f, 0, 0));
		}
	}

	GLRenderer::GetInstance()->DrawDebug_Line(posX, posY, posX + (sign*m_stoppingDistance), posY - (thisTransform->GetScale().y / 2), 1.0f, 0.0f, 0.0f);
	
}

void GroundPatrol::OnStart() {

}

void GroundPatrol::OnUpdate(float dt) {

}

void GroundPatrol::OnFixedUpdate(float dt) {

}
