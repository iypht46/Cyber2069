#include "GroundPatrol.hpp"
#include "Graphic/GLRenderer.h"


GroundPatrol::GroundPatrol()
{
}


GroundPatrol::~GroundPatrol()
{
}

void GroundPatrol::Init() {
	m_stoppingDistance = 25.0f;
	m_speed = 50.0f;
	delay_timer = 3.0f;
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

void GroundPatrol::Patrol(float dt) {
	Physic::RayHit* Ground = nullptr;
	float posX, posY;

	posX = thisTransform->GetPosition().x;
	posY = thisTransform->GetPosition().y;
	float sign = thisTransform->GetScale().x / glm::abs(thisTransform->GetScale().x);

	Ground = &(ps->Raycast(Physic::Ray(posX, posY, (posX + (sign*m_stoppingDistance)), (posY - (thisTransform->GetScale().y / 2.0f))), ps->GetLayerFromString("Platform")));
	GLRenderer::GetInstance()->DrawDebug_Line(posX, posY, (posX + (sign*m_stoppingDistance)), (posY - (thisTransform->GetScale().y / 2.0f)), 1.0f, 0.0f, 0.0f);
	float dir = posX - Ground->position.x;
	ENGINE_INFO("{}", dir);
	if (!Ground->hit) {
		delay_timer -= dt;

		if (dir < 0) {
			if (delay_timer > 0) {
				rb->SetVelocity(glm::vec3(0, 0, 0));
			}
			else {
				thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x), thisTransform->GetScale().y, 1.0f));
				rb->SetVelocity(glm::vec3(m_speed, 0, 0));
				delay_timer = 3.0f;
			}
		}
		else {
			if (delay_timer > 0) {
				rb->SetVelocity(glm::vec3(0, 0, 0));
			}
			else {
				thisTransform->SetScale(glm::vec3(glm::abs(thisTransform->GetScale().x) * -1, thisTransform->GetScale().y, 1.0f));
				rb->SetVelocity(glm::vec3(-m_speed, 0, 0));
				delay_timer = 3.0f;
			}
		}	
		
	}

}

void GroundPatrol::OnStart() {

}

void GroundPatrol::OnUpdate(float dt) {

}

void GroundPatrol::OnFixedUpdate(float dt) {

}
