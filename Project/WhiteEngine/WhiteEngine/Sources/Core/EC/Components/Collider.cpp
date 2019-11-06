#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Physic/Collision.hpp"


	//////////////Base Collider/////////////

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

		if (m_rigidbody)
		{
			m_rigidbody = m_rigidbody;
			m_isStatic = false;
			ComputeMass();
		}
		else
		{
			m_isStatic = true;
			m_rigidbody->m_mass = 0.0f;
			m_rigidbody->m_invMass = 0.0f;
			m_rigidbody->m_gravityScale = 0.0f;
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

	//////////////Circle Collider///////////




