#pragma once

#include <glm/glm.hpp>
//#include "Core/EC/Components/Collider.hpp"
#include <iostream>

//Forward Declaration
class Collider;

namespace Physic
{
	class PhysicScene;
	enum class RESOLVE_TYPE : int
	{
		COLLISION, TRIGGER
	};

	struct AABB
	{
		glm::vec2 m_min;
		glm::vec2 m_max;
	};

	struct Collision
	{
	protected:
		friend class PhysicScene;
		RESOLVE_TYPE m_type;
	public:
		Collider* m_collider;
		Collider* m_otherCollider;

		Collision();
		Collision(Collider* col, Collider* otherCol, RESOLVE_TYPE type);
	};

	static const float RESOLVE_MUL = 10.0f;

	struct Manifold
	{
		
		RESOLVE_TYPE m_type;
		Collider* m_objectA;
		Collider* m_objectB;
		bool m_objAResFlag;
		bool m_objBResFlag;
		float m_penetration;
		glm::vec3 m_normal;

		Manifold(Collider* a, Collider* b, RESOLVE_TYPE type);
		bool CheckCollision();
		void Resolve(float dt);
	};

	

	//For Box to Box
	bool AABBtoAABB(AABB, AABB);

	//typedef void(*CollisionFunc)(Collider*, Collider*);

}
