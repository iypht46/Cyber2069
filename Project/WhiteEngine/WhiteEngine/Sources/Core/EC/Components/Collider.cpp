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

		if (rigid)
		{
			m_rigidbody = rigid;
			m_isStatic = false;
		}
		else
		{
			m_isStatic = true;
		}

		//Set Box Size
		m_width = hW * 2.0f;
		m_height = hH * 2.0f;

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

		//Set Static
		m_isStatic = false;
		
	}

	void BoxCollider::OnAwake()
	{
	}

	void BoxCollider::OnEnable()
	{
	}

	void BoxCollider::OnStart()
	{
	}

	void BoxCollider::OnDisable()
	{
	}

	void BoxCollider::OnUpdate(float dt)
	{
	}

	void BoxCollider::OnFixedUpdate(float dt)
	{
	}

	void BoxCollider::OnDestroy()
	{
	}

	//////////////Circle Collider///////////




