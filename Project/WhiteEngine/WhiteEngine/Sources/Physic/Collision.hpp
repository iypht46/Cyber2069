#pragma once

#include <glm/glm.hpp>
//#include "Core/EC/Components/Collider.hpp"
#include <iostream>
#include <memory>

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
		glm::vec2 m_min; //topleft
		glm::vec2 m_max; //lowerright
	};

	struct Ray
	{
		glm::vec2 origin;
		glm::vec2 end;

		Ray(glm::vec2 origin, glm::vec2 end);
		Ray(glm::vec2 origin, glm::vec2 direction, float distance);
		Ray(float originx, float originy, float endx, float endy);
		float distance();
	};

	struct RayHit
	{
		bool hit;
		glm::vec2 position;
		std::shared_ptr<Collider> collider;

		RayHit(bool hit = false);
		RayHit(glm::vec2, std::shared_ptr<Collider>);
	};

	struct Collision
	{
	protected:
		friend class PhysicScene;
		RESOLVE_TYPE m_type;
	public:
		std::shared_ptr<Collider> m_collider;
		std::shared_ptr<Collider> m_otherCollider;

		Collision();
		Collision(std::shared_ptr<Collider> col, std::shared_ptr<Collider> otherCol, RESOLVE_TYPE type);
	};

	static const float RESOLVE_MUL = 10.0f;

	struct Manifold
	{
		
		RESOLVE_TYPE m_type;
		std::shared_ptr<Collider> m_objectA;
		std::shared_ptr<Collider> m_objectB;
		bool m_objAResFlag;
		bool m_objBResFlag;
		float m_penetration;
		glm::vec3 m_normal;

		Manifold(std::shared_ptr<Collider> a, std::shared_ptr<Collider> b, RESOLVE_TYPE type);
		bool CheckCollision();
		void Resolve(float dt);
	};

	

	//For Box to Box
	bool AABBtoAABB(AABB, AABB);
	RayHit RaytoRay(Ray, Ray);
	RayHit RaytoAABB(Ray, AABB);

	//typedef void(*CollisionFunc)(Collider*, Collider*);
}
