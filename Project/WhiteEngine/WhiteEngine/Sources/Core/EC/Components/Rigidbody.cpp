#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"

#include <iostream>

using namespace glm;

Rigidbody::Rigidbody()
{
	m_gravityScale = 1.0f;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_velocity = vec3(0.0f);
	m_mass = 1;
	m_drag = 0.0f;

	//Factory<Rigidbody>::Add(this);
}

//Create BoxCollider
void Rigidbody::Init()
{
	//assign Collider
	m_collider = GetGameObject()->GetComponent<Collider>();
	//assign transform
	m_transform = GetGameObject()->m_transform.get();

	//assign self to physicsscene
	Physic::PhysicScene::GetInstance()->Add(this);
}

//Create BoxCollider
void Rigidbody::Init(float hW, float hH)
{
	//Create new box collider
	BoxCollider* col = GetGameObject()->AddComponent<BoxCollider>();
	//Init Box
	col->Init(hW, hH, this);
	//Set Collider
	m_collider = col;
	m_transform = GetGameObject()->m_transform.get();
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::SetVelocity(vec3 velocity)
{
	m_velocity = velocity;
}

//not set that axis if target velocity in that axis is 0
void Rigidbody::SetVelocity0Masked(vec3 velocity) {
	for (int i = 0; i < 3; i++) {
		if (velocity[i] != 0.0f) {
			m_velocity[i] = velocity[i];
		}
	}
}

glm::vec3 Rigidbody::GetVelocity()
{
	return m_velocity;
}

float Rigidbody::GetGravityScale()
{
	return m_gravityScale;
}

void Rigidbody::SetGravityScale(float scale)
{
	m_gravityScale = scale;
}


void Rigidbody::AddForce(vec3 force)
{
	m_velocity += (force / m_mass);
}

void Rigidbody::AddRelativeForce(vec3 force)
{
	vec3 AbsoluteForce;
	float Rotation(radians(m_transform->GetRotation()));
	AbsoluteForce.x = (force.x * cos(Rotation)) - (force.y * sin(Rotation));
	AbsoluteForce.y = (force.x * sin(Rotation)) + (force.y * cos(Rotation));
	AbsoluteForce.z = 0;

	AddForce(AbsoluteForce);
}

void Rigidbody::UpdateTransform(float dt)
{
	m_velocity = m_velocity * (1 - m_drag);
	m_transform->Translate(m_velocity * dt);
}

void Rigidbody::SetMass(float mass) {
	if (automass) {
		m_mass = mass;
		m_invMass = 1.0 / m_mass;
	}
}

float Rigidbody::GetInvMass(void)
{
	return m_invMass;
}

float Rigidbody::GetMass(void)
{
	return m_mass;
}

void Rigidbody::SetDrag(float drag) {
	m_drag = drag;
}

float Rigidbody::GetDrag() {
	return m_drag;
}
