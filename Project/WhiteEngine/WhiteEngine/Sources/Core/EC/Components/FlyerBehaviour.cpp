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
	Rigidbody* rb = m_gameObject->GetComponent<Rigidbody>();
	glm::vec3 myVel = m_gameObject->GetComponent<Rigidbody>()->GetVelocity();
	glm::vec3 direction = glm::normalize(player->GetPosition() - flyer->GetPosition());
	float rot = glm::cross(glm::normalize(myVel), direction).z;
	flyer->SetRotation(flyer->GetRotation() + (rot * rotRate * dt));
	
	rb->SetVelocity(glm::vec3(flyerSpeed * glm::cos(flyer->GetRotation() + rotAngle),
							  flyerSpeed * glm::sin(flyer->GetRotation() + rotAngle), 0));
	
	if (direction.x > 0) {
		flyer->SetScale(glm::vec3(glm::abs(flyer->GetScale().x) * -1, flyer->GetScale().y, 1.0f));

	}
	else {
		flyer->SetScale(glm::vec3(glm::abs(flyer->GetScale().x), flyer->GetScale().y, 1.0f));;
	}


}

void FlyerBehaviour::OnFixedUpdate(float dt)
{
}
