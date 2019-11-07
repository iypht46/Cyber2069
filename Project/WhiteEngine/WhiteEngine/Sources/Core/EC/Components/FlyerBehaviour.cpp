#include "FlyerBehaviour.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

FlyerBehaviour::FlyerBehaviour() {

}

void FlyerBehaviour::SetPlayer(Transform& player) {
	this->player = &player;
	flyer = &m_gameObject->m_transform;
}

void FlyerBehaviour::OnAwake()
{
}

void FlyerBehaviour::OnEnable()
{
}

void FlyerBehaviour::OnStart()
{
}

void FlyerBehaviour::OnDisable()
{
}

void FlyerBehaviour::OnUpdate(float dt)
{
	float degree;
	glm::vec3 direction(player->GetPosition() - flyer->GetPosition());
	direction /= glm::length(direction);
	m_gameObject->GetComponent<Rigidbody>()->SetVelocity(direction * MOVE_SPEED_FLYER);
	degree = glm::atan(direction.y, direction.x);
	degree = glm::degrees(degree);
	flyer->SetRotation(degree);
	if (direction.x > 0) {
		flyer->SetScale(glm::vec3(glm::abs(flyer->GetScale().x) * -1, flyer->GetScale().y, 1.0f));
	
	}
	else {
		flyer->SetScale(glm::vec3(glm::abs(flyer->GetScale().x), flyer->GetScale().y, 1.0f));
		flyer->SetRotation(degree - 180);
	}



}

void FlyerBehaviour::OnFixedUpdate(float dt)
{
}
