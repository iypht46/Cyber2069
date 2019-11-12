#include "Collision.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Transform.hpp"
#include "Core/Logger.hpp"
#include <iostream>
namespace Physic
{

	//bool AABBtoAABB(Manifold* m)
	//{
	//	//Cast Collider to BoxCollider
	//	BoxCollider* objectA = dynamic_cast<BoxCollider*>(m->m_objectA);
	//	BoxCollider* objectB = dynamic_cast<BoxCollider*>(m->m_objectB);
	//	//AABB data
	//	AABB a, b;
	//	//Generate AABB data from the collider data
	//	objectA->ComputeAABB(a);
	//	objectB->ComputeAABB(b);

	//	if (a.m_max.x < b.m_min.x)
	//	{
	//		return false;
	//	}
	//	else
	//	{
	//		//Collide Right
	//		ENGINE_INFO("Collision Right");
	//		m->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);
	//	}

	//	if (a.m_min.x > b.m_max.x)
	//	{
	//		return false;
	//	}
	//	else
	//	{
	//		//Collide Left
	//		ENGINE_INFO("Collision Left");
	//		m->m_normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	//	}

	//	if (a.m_max.y > b.m_min.y)
	//	{
	//		return false;
	//	}
	//	else
	//	{
	//		//Collide Down
	//		ENGINE_INFO("Collision Down");
	//		m->m_normal = glm::vec3(0.0f, -1.0f, 0.0f);
	//	}

	//	if (a.m_min.y < b.m_max.y)
	//	{
	//		return false;
	//	}
	//	else
	//	{
	//		//Collide Up
	//		ENGINE_INFO("Collision Up");
	//		m->m_normal = glm::vec3(0.0f, 1.0f, 0.0f);
	//	}

	//	//std::cout << "Collided\n";
	//	return true;
	//}

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
		if (x_overlap > y_overlap)
		{
			// Point towards B knowing that n points from A to B
			if (n.x < 0)
				m->m_normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			else
				m->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);

			m->m_penetration = x_overlap;
			//std::cout << "Collided\n";
		}
		else
		{
			// Point toward B knowing that n points from A to B
			if (n.y < 0)
				m->m_normal = glm::vec3(0.0f, -1.0f, 0.0f);
			else
				m->m_normal = glm::vec3(0.0f, 1.0f, 0.0f);

			m->m_penetration = y_overlap;
			//std::cout << "Collided\n";
		}

		return true;
	}

	bool Manifold::CheckCollision()
	{
		if (m_objectA && m_objectB)
		{
			if (m_objectA->GetType() == ColliderType::BOX
				&& m_objectB->GetType() == ColliderType::BOX)
			{
				/*std::cout << "Object A:\n";
				std::cout << "Min X: " << a.m_min.x << " , Y: " << a.m_min.y << std::endl;
				std::cout << "Max X: " << a.m_max.x << " , Y: " << a.m_max.y << std::endl;

				std::cout << "Object B:\n";
				std::cout << "Min X: " << b.m_min.x << " , Y: " << b.m_min.y << std::endl;
				std::cout << "Max X: " << b.m_max.x << " , Y: " << b.m_max.y << std::endl;*/

				return (AABBtoAABB(this));
				//return (AABBtoAABB(this));
			}
		}
		else
		{
			ENGINE_ERROR("Object reference in Manifold is NULL");
		}


		return false;
	}

	void Manifold::Resolve(float dt)
	{
		if (!m_objectA->IsStatic())
		{
			m_objectA->m_rigidbody->SetVelocity(-(m_normal * (m_penetration * 10.0f)));
			m_objectA->m_rigidbody->UpdateTransform(dt);
		}

		if (!m_objectB->IsStatic())
		{
			m_objectB->m_rigidbody->SetVelocity(m_normal * (m_penetration * 10.0f));
			m_objectB->m_rigidbody->UpdateTransform(dt);
		}


	}
}
