#pragma once

#include <glm/glm.hpp>
//#include "Core/EC/Components/Collider.hpp"
#include <iostream>

//Forward Declaration
class Collider;

namespace Physic
{
	struct AABB
	{
		glm::vec2 m_min;
		glm::vec2 m_max;
	};

	struct Collision
	{
		Collider* m_collider;
		Collider* m_otherCollider;

		Collision(Collider* col, Collider* otherCol);
	};

	struct Manifold
	{
		Collider* m_objectA;
		Collider* m_objectB;
		float m_penetration;
		glm::vec3 m_normal;

		Manifold(Collider* a, Collider* b) : m_objectA(a), m_objectB(b) {}
		bool CheckCollision();
		void Resolve(float dt);
	};

	//For Box to Box
	bool AABBtoAABB(AABB, AABB);

	//typedef void(*CollisionFunc)(Collider*, Collider*);

}
