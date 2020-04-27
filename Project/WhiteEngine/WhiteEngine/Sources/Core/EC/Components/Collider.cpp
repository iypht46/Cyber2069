#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Physic/Collision.hpp"
#include "Core/Logger.hpp"


	//////////////Base Collider/////////////
	Collider::Collider() {
		Factory<Collider>::Add(this);
	}

	Collider::~Collider() { }

	void Collider::Init() {
		//Set Rigidbody
		m_rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		Physic::PhysicScene::GetInstance()->Add(this, GetGameObject()->Layer);
	}

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
			GetGameObject()->CollisionEnter(msg.m_collision);
			break;
		case COL_STATE::STAY:
			GetGameObject()->CollisionStay(msg.m_collision);
			break;
		case COL_STATE::EXIT:
			GetGameObject()->CollisionExit(msg.m_collision);
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
			GetGameObject()->TriggerEnter(msg.m_collision);
			break;
		case COL_STATE::STAY:
			GetGameObject()->TriggerStay(msg.m_collision);
			break;
		case COL_STATE::EXIT:
			GetGameObject()->TriggerExit(msg.m_collision);
			m_collisionState = COL_STATE::NONE;
			break;
		default:
			break;
		}

		m_hasCollided = false;
	}


	//////////////Box Collider//////////////
	BoxCollider::BoxCollider() : Collider(COLLIDER_TYPE::BOX) {
		Factory<BoxCollider>::Add(this);
	}

	void BoxCollider::Init() {

		Collider::Init();

		//Set Box Size
		m_colliderScale.x = m_halfWidth / m_gameObject->m_transform->GetScale().x;
		m_colliderScale.y = m_halfHeight / m_gameObject->m_transform->GetScale().y;

		if (m_rigidbody)
		{
			m_rigidbody = m_rigidbody;
			m_isStatic = false;

			if (m_rigidbody->automass) {
				ComputeMass();
			}
		}
		else
		{
			m_isStatic = true;
		}
	}

	void BoxCollider::ReSize(float hW, float hH) {
		m_halfWidth = hW;
		m_halfHeight = hH;
		m_colliderScale.x = hW / m_gameObject->m_transform->GetScale().x;
		m_colliderScale.y = hH / m_gameObject->m_transform->GetScale().y;

		if (m_rigidbody)
		{
			m_rigidbody = m_rigidbody;
			m_isStatic = false;

			if (m_rigidbody->automass) {
				ComputeMass();
			}
		}
		else
		{
			m_isStatic = true;
		}
	}

	void BoxCollider::ReScale(float scaleX, float scaleY) {
		m_halfWidth = (m_gameObject->m_transform->GetScale().x * scaleX) / 2;
		m_halfHeight = (m_gameObject->m_transform->GetScale().y * scaleY) / 2;
		m_colliderScale.x = scaleX;
		m_colliderScale.y = scaleY;

		if (m_rigidbody)
		{
			m_rigidbody = m_rigidbody;
			m_isStatic = false;

			if (m_rigidbody->automass) {
				ComputeMass();
			}
		}
		else
		{
			m_isStatic = true;
		}
	}

	void BoxCollider::Init(float hW, float hH, Rigidbody* rigid)
	{
		//Set Transform
		//m_transform = GetGameObject()->m_transform.get();
		//Set Rigidbody
		m_rigidbody = rigid;
		//Set Box Size
		m_halfWidth = hW;
		m_halfHeight = hH;
		m_colliderScale.x = hW / m_gameObject->m_transform->GetScale().x;
		m_colliderScale.y = hH / m_gameObject->m_transform->GetScale().y;

		ComputeMass();
		//Set Static
		m_isStatic = false;
	}

	void BoxCollider::ComputeAABB(Physic::AABB& a)
	{
		a.m_min.x = m_gameObject->m_transform->GetPosition().x - m_halfWidth;
		a.m_min.y = m_gameObject->m_transform->GetPosition().y + m_halfHeight;
		a.m_max.x = m_gameObject->m_transform->GetPosition().x + m_halfWidth;
		a.m_max.y = m_gameObject->m_transform->GetPosition().y - m_halfHeight;

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




