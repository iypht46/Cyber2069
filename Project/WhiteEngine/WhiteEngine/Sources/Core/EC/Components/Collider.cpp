#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Physic/Collision.hpp"
#include "Core/Logger.hpp"


	//////////////Base Collider/////////////

	COLLIDER_TYPE Collider::GetType()
	{
		return m_colliderType;
	}

	bool Collider::IsStatic()
	{
		return m_isStatic;
	}

	COL_STATE Collider::GetCollisionState()
	{
		return m_collisionState;
	}

	void Collider::HandleMessage(const Core::Collision& msg)
	{
		//TODO: Call all OnCollisionStart/Exit for every script
		switch (m_collisionState)
		{
		case COL_STATE::ENTER:
			m_gameObject->CollisionEnter(msg.m_collision);
			break;
		case COL_STATE::STAY:
			m_gameObject->CollisionStay(msg.m_collision);
			break;
		case COL_STATE::EXIT:
			m_gameObject->CollisionExit(msg.m_collision);
			m_collisionState = COL_STATE::NONE;
			break;
		default:
			break;
		}

		
	}

	void Collider::HandleMessage(const Core::Trigger& msg)
	{
		//TODO: Call all OnTriggerStart/Exit for every script
		switch (m_collisionState)
		{
		case COL_STATE::ENTER:
			m_gameObject->TriggerEnter(msg.m_collision);
			break;
		case COL_STATE::STAY:
			m_gameObject->TriggerStay(msg.m_collision);
			break;
		case COL_STATE::EXIT:
			m_gameObject->TriggerExit(msg.m_collision);
			m_collisionState = COL_STATE::NONE;
			break;
		default:
			break;
		}

		m_hasCollided = false;
	}


	//////////////Box Collider//////////////
	void BoxCollider::Init(float hW, float hH)
	{
		//Set Transform
		//m_transform = m_gameObject->GetTransform();
		//Set Rigidbody
		m_rigidbody = m_gameObject->GetComponent<Rigidbody>();
		//Set Transform
		m_transform = &m_gameObject->m_transform;

		//Set Box Size
		m_halfWidth = hW;
		m_halfHeight = hH;
		m_colliderScale.x = hW / m_transform->GetScale().x;
		m_colliderScale.y = hH / m_transform->GetScale().y;

		if (m_rigidbody)
		{
			m_rigidbody = m_rigidbody;
			m_isStatic = false;
			ComputeMass();
		}
		else
		{
			m_isStatic = true;
		}
	}

	void BoxCollider::Init(float hW, float hH, Rigidbody* rigid)
	{
		//Set Transform
		m_transform = &m_gameObject->m_transform;
		//Set Rigidbody
		m_rigidbody = rigid;
		//Set Box Size
		m_halfWidth = hW;
		m_halfHeight = hH;
		m_colliderScale.x = hW / m_transform->GetScale().x;
		m_colliderScale.y = hH / m_transform->GetScale().y;

		ComputeMass();
		//Set Static
		m_isStatic = false;
		
	}

	void BoxCollider::ComputeAABB(Physic::AABB& a)
	{
		a.m_min.x = m_transform->GetPosition().x - m_halfWidth;
		a.m_min.y = m_transform->GetPosition().y + m_halfHeight;
		a.m_max.x = m_transform->GetPosition().x + m_halfWidth;
		a.m_max.y = m_transform->GetPosition().y - m_halfHeight;

	}

	void BoxCollider::ComputeMass()
	{
		m_rigidbody->m_mass = ((m_halfWidth*2) * (m_halfHeight*2)) * m_density;
		m_rigidbody->m_invMass = 1.0 / m_rigidbody->m_mass;
	}

	glm::vec3 BoxCollider::GetColliderScale()
	{
		return m_colliderScale;
	}



	//////////////Circle Collider///////////




