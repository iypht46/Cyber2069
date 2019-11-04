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
	glm::vec3 direction(player->GetPosition() - flyer->GetPosition());
	direction /= glm::length(direction);

	flyer->Translate(direction * MOVE_SPEED_FLYER *dt);
	//if (flyer->GetPosition().x
	//	>= player->GetPosition().x) {
	//	
	//}
	//
	//if (flyer->GetPosition().y >= player->GetPosition().y) {
	//	flyer->Translate(glm::vec3(0.0f, -MOVE_SPEED_FLYER * dt, 0.0f));
	//}

	//if (flyer->GetPosition().x <= player->GetPosition().x) {
	//	flyer->Translate(glm::vec3(MOVE_SPEED_FLYER * dt, 0.0f, 0.0f));
	//}

	//if (flyer->GetPosition().y <= player->GetPosition().y) {
	//	flyer->Translate(glm::vec3(0.0f, MOVE_SPEED_FLYER * dt, 0.0f));
	//}

	//ENGINE_INFO("Behavior Update");
}

void FlyerBehaviour::OnFixedUpdate(float dt)
{
}
