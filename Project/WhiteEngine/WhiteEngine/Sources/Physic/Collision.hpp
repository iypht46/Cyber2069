#pragma once

#include <glm/glm.hpp>
#include "Core/EC/Components/Collider.hpp"


namespace Physic
{
	struct AABB
	{
		glm::vec2 m_min;
		glm::vec2 m_max;
	};

	struct Manifold
	{
		Collider* m_objectA;
		Collider* m_objectB;
		float m_penetration;
		glm::vec2 m_normal;

		Manifold(Collider* a, Collider* b) : m_objectA(a), m_objectB(b) {}
		bool CheckCollision();
		void Resolve();
	};

	//For Box to Box
	bool AABBtoAABB(AABB, AABB);

	typedef void(*CollisionFunc)(Collider*, Collider*);

}
