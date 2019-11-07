#include "Collision.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Transform.hpp"
#include "Core/Logger.hpp"
#include <iostream>
namespace Physic
{

	bool AABBtoAABB(AABB a, AABB b)
	{

		if (a.m_max.x < b.m_min.x || a.m_min.x > b.m_max.x)
		{
			//std::cout << "Not Collided X\n";
			return false;
		}

		if (a.m_max.y > b.m_min.y || a.m_min.y < b.m_max.y)
		{
			//std::cout << "Not Collided Y\n";
			return false;
		}

		//std::cout << "Collided\n";
		return true;
	}

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

			// Calculate half extents along x axis for each object
		float a_extent = (abox.m_max.x - abox.m_min.x) / 2;
		float b_extent = (bbox.m_max.x - bbox.m_min.x) / 2;

		// Calculate overlap on x axis
		float x_overlap = a_extent + b_extent - abs(n.x);

		// SAT test on x axis
		if (x_overlap > 0)
		{
			// Calculate half extents along x axis for each object
			float a_extent = (abox.m_max.y - abox.m_min.y) / 2;
			float b_extent = (bbox.m_max.y - bbox.m_min.y) / 2;

			// Calculate overlap on y axis
			float y_overlap = a_extent + b_extent - abs(n.y);

			// SAT test on y axis
			if (y_overlap > 0)
			{
				// Find out which axis is axis of least penetration
				if (x_overlap > y_overlap)
				{
					// Point towards B knowing that n points from A to B
					if (n.x < 0)
						m->m_normal = glm::vec2(-1, 0);
					else
						m->m_normal = glm::vec2(0, 0);
					
					m->m_penetration = x_overlap;
					std::cout << "Collided\n";
					return true;
				}
				else
				{
					// Point toward B knowing that n points from A to B
					if (n.y < 0)
						m->m_normal = glm::vec2(0, -1);
					else
						m->m_normal = glm::vec2(0, 1);

					m->m_penetration = y_overlap;
					std::cout << "Collided\n";
					return true;
				}
			}
		}

		return false;
	}

	bool Manifold::CheckCollision()
	{
		if (m_objectA && m_objectB)
		{
			if (m_objectA->GetType() == ColliderType::BOX
				&& m_objectB->GetType() == ColliderType::BOX)
			{
				//AABB data
				AABB a, b;
				//Cast Collider to BoxCollider
				BoxCollider* objectA = dynamic_cast<BoxCollider*>(m_objectA);
				BoxCollider* objectB = dynamic_cast<BoxCollider*>(m_objectB);
				//Generate AABB data from the collider data
				objectA->ComputeAABB(a);
				objectB->ComputeAABB(b);

				/*std::cout << "Object A:\n";
				std::cout << "Min X: " << a.m_min.x << " , Y: " << a.m_min.y << std::endl;
				std::cout << "Max X: " << a.m_max.x << " , Y: " << a.m_max.y << std::endl;

				std::cout << "Object B:\n";
				std::cout << "Min X: " << b.m_min.x << " , Y: " << b.m_min.y << std::endl;
				std::cout << "Max X: " << b.m_max.x << " , Y: " << b.m_max.y << std::endl;*/

				return (AABBtoAABB(a, b));
				//return (AABBtoAABB(this));
			}
		}
		else
		{
			ENGINE_ERROR("Object reference in Manifold is NULL");
		}
		

		return false;
	}

	void Manifold::Resolve()
	{
		if (m_objectA->IsStatic())
		{

		}

		if (m_objectB->IsStatic())
		{

		}
	}
}