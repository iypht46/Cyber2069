#include "GroundPatrol.hpp"
#include "Graphic/GLRenderer.h"


GroundPatrol::GroundPatrol()
{
}


GroundPatrol::~GroundPatrol()
{
}

void GroundPatrol::Init() {
	m_stoppingDistance = 50.0f;
	m_speed = 10.0f;
	thisTransform = &(m_gameObject->m_transform);
	rb = m_gameObject->GetComponent<Rigidbody>();
	ps = Physic::PhysicScene::GetInstance();
	rb->SetVelocity(glm::vec3(m_speed, 0, 0));
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
	float sign = thisTransform->GetScale().x / glm::abs(thisTransform->GetScale().x);

	Ground = ps->Raycast(Physic::Ray(posX, posY, (posX + (sign*m_stoppingDistance)), (posY - (thisTransform->GetScale().y / 2.0f))), ps->GetLayerFromString("Platform"));
	GLRenderer::GetInstance()->DrawDebug_Line(posX, posY, (posX + (sign*m_stoppingDistance)), (posY - (thisTransform->GetScale().y / 2.0f)), 1.0f, 0.0f, 0.0f);

	//ENGINE_INFO("{}", (int)Ground);
	if (Ground == nullptr) {
		ENGINE_INFO("HELLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
		if (rb->GetVelocity().x < 0) {
			thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x), thisTransform->GetScale().y, 1.0f));
			rb->SetVelocity(glm::vec3(m_speed, 0, 0));
			

		}
		else {
			thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x) * - 1, thisTransform->GetScale().y, 1.0f));
			rb->SetVelocity(glm::vec3(-m_speed, 0, 0));
		}
	}
	else {
		ENGINE_INFO("HITO");
	}

	
}

void GroundPatrol::OnStart() {

}

void GroundPatrol::OnUpdate(float dt) {

}

void GroundPatrol::OnFixedUpdate(float dt) {

}
