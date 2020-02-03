#include "Collision.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Transform.hpp"
#include "Core/Logger.hpp"
#include <iostream>
namespace Physic
{

	bool AABBtoAABB(Manifold* m)
	{
		//Cast Collider to BoxCollider
		BoxCollider* A = dynamic_cast<BoxCollider*>(m->m_objectA);
		BoxCollider* B = dynamic_cast<BoxCollider*>(m->m_objectB);

		// Vector from A to B
		glm::vec3 n = B->m_transform->GetPosition() - A->m_transform->GetPosition();


		AABB abox, bbox;
		A->ComputeAABB(abox);
		B->ComputeAABB(bbox);

		if (abox.m_max.x < bbox.m_min.x || abox.m_min.x > bbox.m_max.x) return false;
		if (abox.m_max.y > bbox.m_min.y || abox.m_min.y < bbox.m_max.y) return false;

			// Calculate half extents along x axis for each object
		float a_extent = (abox.m_max.x - abox.m_min.x) / 2;
		float b_extent = (bbox.m_max.x - bbox.m_min.x) / 2;

		// Calculate overlap on x axis
		float x_overlap = a_extent + b_extent - abs(n.x);

		/*std::cout << "X: \na_extent: " << a_extent << std::endl;
		std::cout << "b_extent: " << b_extent << std::endl;
		std::cout << "x_overlap: " << x_overlap << std::endl;*/

		// Calculate half extents along x axis for each object
		a_extent = abs(abox.m_max.y - abox.m_min.y) / 2;
		b_extent = abs(bbox.m_max.y - bbox.m_min.y) / 2;

		// Calculate overlap on y axis
		float y_overlap = a_extent + b_extent - abs(n.y);

		/*std::cout << "Y: \na_extent: " << a_extent << std::endl;
		std::cout << "b_extent: " << b_extent << std::endl;
		std::cout << "y_overlap: " << y_overlap << std::endl;*/

		// Find out which axis is axis of least penetration
		if (x_overlap < y_overlap)
		{
			// Point towards B knowing that n points from A to B
			if (n.x < 0)
				m->m_normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			else
				m->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);

			m->m_penetration = x_overlap;
		}
		else
		{
			// Point toward B knowing that n points from A to B
			if (n.y < 0)
				m->m_normal = glm::vec3(0.0f, -1.0f, 0.0f);
			else
				m->m_normal = glm::vec3(0.0f, 1.0f, 0.0f);

			m->m_penetration = y_overlap;
		}

		return true;
	}

	//Default Constructor
	Collision::Collision() {}
	//Constructor
	Collision::Collision(Collider* col, Collider* otherCol, RESOLVE_TYPE type)
		: m_collider(col), m_otherCollider(otherCol), m_type(type) {}
	
	Manifold::Manifold(Collider* a, Collider* b, RESOLVE_TYPE type) : m_objectA(a), m_objectB(b), m_type(type)
	{
		m_objAResFlag = !a->IsStatic();
		m_objBResFlag = !b->IsStatic();
	}

	bool Manifold::CheckCollision()
	{
		if (m_objectA && m_objectB)
		{
			if (m_objectA->GetType() == COLLIDER_TYPE::BOX
				&& m_objectB->GetType() == COLLIDER_TYPE::BOX)
			{
				return (AABBtoAABB(this));
			}
		}
		else
		{
			ENGINE_ERROR("Object reference in Manifold is NULL");
		}

		return false;
	}
#define RE_MUL 10.0f
	void Manifold::Resolve(float dt)
	{
		if (m_type == RESOLVE_TYPE::COLLISION)
		{
			glm::vec3 resultVec = m_normal * (m_penetration * RESOLVE_MUL);

			//If both object is moving (Both is not static) then check its velocity
			if (m_objAResFlag && m_objBResFlag)
			{
				glm::vec3 relativeVel = m_objectB->m_rigidbody->GetVelocity() - m_objectA->m_rigidbody->GetVelocity();

				//If object a is going in a different direction then do not resolve
				if (glm::dot(relativeVel, m_normal) > 0)
				{
					return;
				}
			}
			
			if (m_objAResFlag)
			{
				glm::vec3 ResolvedVelocity;

				for (int i = 0; i < 3; i++) {
					if (m_normal[i] == 0.0f) {
						ResolvedVelocity[i] = m_objectA->m_rigidbody->GetVelocity()[i];
					}
					else {
						ResolvedVelocity[i] = -resultVec[i];
						//ENGINE_INFO(i);
					}
				}

				m_objectA->m_rigidbody->SetVelocity(ResolvedVelocity);
			}

			if (m_objBResFlag)
			{
				glm::vec3 ResolvedVelocity;

				for (int i = 0; i < 3; i++) {
					if (m_normal[i] == 0.0f) {
						ResolvedVelocity[i] = m_objectB->m_rigidbody->GetVelocity()[i];
					}
					else {
						ResolvedVelocity[i] = resultVec[i];
						//ENGINE_INFO(i);
					}
				}

				m_objectB->m_rigidbody->SetVelocity(ResolvedVelocity);
			}
		}
		
		//glm::vec3 resultVec = m_normal * (m_penetration * RESOLVE_MUL);
		//
		//if (!m_objectA->IsStatic() && !m_objectB->IsStatic())
		//{
		//	glm::vec3 relativeVel = m_objectB->m_rigidbody->GetVelocity() - m_objectA->m_rigidbody->GetVelocity();

		//	//If object a is going in a different direction then do not resolve
		//	if (glm::dot(relativeVel, m_normal) > 0)
		//	{
		//		return;
		//	}

		//	//Resolve only resolve type is collision
		//	if (m_type == RESOLVE_TYPE::COLLISION)
		//	{
		//		glm::vec3 ResolvedVelocity;

		//		for (int i = 0; i < 3; i++) {
		//			if (m_normal[i] == 0.0f) {
		//				ResolvedVelocity[i] = m_objectA->m_rigidbody->GetVelocity()[i];
		//			}
		//			else {
		//				ResolvedVelocity[i] = -resultVec[i];
		//				//ENGINE_INFO(i);
		//			}
		//		}

		//		m_objectA->m_rigidbody->SetVelocity(ResolvedVelocity);
		//		//ENGINE_INFO(m_objectA->m_rigidbody->GetVelocity()[0]);
		//		//m_objectA->m_rigidbody->SetVelocity(resultVec);
		//	}

		//	if (m_type == RESOLVE_TYPE::COLLISION)
		//	{
		//		glm::vec3 ResolvedVelocity;

		//		for (int i = 0; i < 3; i++) {
		//			if (m_normal[i] == 0.0f) {
		//				ResolvedVelocity[i] = m_objectB->m_rigidbody->GetVelocity()[i];
		//			}
		//			else {
		//				ResolvedVelocity[i] = resultVec[i];
		//				//ENGINE_INFO(i);
		//			}
		//		}

		//		m_objectB->m_rigidbody->SetVelocity(ResolvedVelocity);
		//		//ENGINE_INFO(m_objectA->m_rigidbody->GetVelocity()[0]);

		//		//m_objectB->m_rigidbody->SetVelocity(resultVec);
		//	}
		//}
		//else
		//{
		//	if (!m_objectA->IsStatic())
		//	{
		//		if (m_type == RESOLVE_TYPE::COLLISION) 
		//		{
		//			glm::vec3 ResolvedVelocity;

		//			for (int i = 0; i < 3; i++) {
		//				if (m_normal[i] == 0.0f) {
		//					ResolvedVelocity[i] = m_objectA->m_rigidbody->GetVelocity()[i];
		//				}
		//				else {
		//					ResolvedVelocity[i] = -resultVec[i];
		//					//ENGINE_INFO(i);
		//				}
		//			}

		//			m_objectA->m_rigidbody->SetVelocity(ResolvedVelocity);
		//			//ENGINE_INFO(m_objectA->m_rigidbody->GetVelocity()[0]);
		//		}
		//	}
		//	else
		//	{
		//		if (m_type == RESOLVE_TYPE::COLLISION) 
		//		{
		//			glm::vec3 ResolvedVelocity;

		//			for (int i = 0; i < 3; i++) 
		//			{
		//				if (m_normal[i] == 0.0f) 
		//				{
		//					ResolvedVelocity[i] = m_objectB->m_rigidbody->GetVelocity()[i];
		//				}
		//				else 
		//				{
		//					ResolvedVelocity[i] = resultVec[i];
		//					//ENGINE_INFO(i);
		//				}
		//			}

		//			m_objectB->m_rigidbody->SetVelocity(ResolvedVelocity);
		//			//ENGINE_INFO(m_objectA->m_rigidbody->GetVelocity()[0]);
		//		}
		//	}
		//}


	}
}
