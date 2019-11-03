#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"


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
		Rigidbody* rigid = m_gameObject->GetComponent<Rigidbody>();

		//Set Box Size
		m_width = hW * 2.0f;
		m_height = hH * 2.0f;

		if (rigid)
		{
			m_rigidbody = rigid;
			m_isStatic = false;
			ComputeMass();
		}
		else
		{
			m_isStatic = true;
			rigid->m_mass = 0.0f;
			rigid->m_invMass = 0.0f;
			rigid->m_gravityScale = 0.0f;
		}

		

	}

	void BoxCollider::Init(float hW, float hH, Rigidbody* rigid)
	{
		//Set Transform
		//m_transform = m_gameObject->GetTransform();
		//Set Rigidbody
		m_rigidbody = rigid;
		//Set Box Size
		m_width = hW * 2.0f;
		m_height = hH * 2.0f;
		ComputeMass();
		//Set Static
		m_isStatic = false;
		
	}

	void BoxCollider::ComputeMass()
	{
		m_rigidbody->m_mass = (m_width * m_height) * m_density;
		m_rigidbody->m_invMass = 1.0 / m_rigidbody->m_mass;
	}

	//////////////Circle Collider///////////




