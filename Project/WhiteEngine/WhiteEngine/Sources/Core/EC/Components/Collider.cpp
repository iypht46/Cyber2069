#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/GameObject.hpp"

namespace Core
{
	//////////////Base Collider/////////////

	void Collider::AttachRigidbody(Rigidbody& rigid)
	{
		m_rigidbody = &rigid;
	}

	void Collider::SetTrigger(bool i)
	{
		m_isTrigger = i;
	}

	ColliderType Collider::GetType()
	{
		return m_colliderType;
	}

	bool Collider::IsStatic()
	{
		return m_isStatic;
	}

	//////////////Box Collider//////////////
	BoxCollider::BoxCollider(glm::vec2 size) : Collider(ColliderType::BOX) 
	{
		m_size = size;
		float halfW = size.x / 2;
		float halfH = size.y / 2;
		//Lower Left
		m_vertices[0] = glm::vec2(-halfW, -halfH);
		//Lower Right
		m_vertices[1] = glm::vec2(halfW, -halfH);
		//Upper Right
		m_vertices[2] = glm::vec2(halfW, halfH);
		//Upper Left
		m_vertices[3] = glm::vec2(-halfW, halfH);
	}

	BoxCollider::BoxCollider(glm::vec2 size, Rigidbody& rigid) : Collider(ColliderType::BOX)
	{
		//Set Box Size
		m_size = size;
		float halfW = size.x / 2;
		float halfH = size.y / 2;
		//Lower Left
		m_vertices[0] = glm::vec2(-halfW, -halfH);
		//Lower Right
		m_vertices[1] = glm::vec2(halfW, -halfH);
		//Upper Right
		m_vertices[2] = glm::vec2(halfW, halfH);
		//Upper Left
		m_vertices[3] = glm::vec2(-halfW, halfH);

		//Attach Rigidbody
		AttachRigidbody(rigid);
		
	}

	//////////////Circle Collider///////////


}


