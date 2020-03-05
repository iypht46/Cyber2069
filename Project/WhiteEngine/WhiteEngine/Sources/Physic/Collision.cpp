#include "Collision.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"
#include <iostream>

namespace Physic
{
	Ray::Ray(glm::vec2 from, glm::vec2 to) {
		origin = from;
		end = to;
	}

	Ray::Ray(glm::vec2 from, glm::vec2 direction, float length) {
		origin = from;
		end = glm::normalize(from + direction) * length;
	}

	Ray::Ray(float originx, float originy, float endx, float endy) {
		origin = glm::vec2(originx, originy);
		end = glm::vec2(endx, endy);
	}

	float Ray::distance() {
		return glm::length(end - origin);
	}

	RayHit::RayHit(bool hit) {
		this->hit = hit;
		collider = nullptr;
	}

	RayHit::RayHit(glm::vec2 pos, Collider* col) {
		hit = true;
		position = pos;
		collider = col;
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

	RayHit RaytoRay(Ray rayA, Ray rayB) {
		float x1 = rayA.origin.x;
		float x2 = rayA.end.x;
		float x3 = rayB.origin.x;
		float x4 = rayB.end.x;
		float y1 = rayA.origin.y;
		float y2 = rayA.end.y;
		float y3 = rayB.origin.y;
		float y4 = rayB.end.y;

		float A1 = y2 - y1;
		float B1 = x1 - x2;
		float C1 = (A1*x1) + (B1*y1);
		float A2 = y4 - y3;
		float B2 = x3 - x4;
		float C2 = (A2*x3) + (B2*y3);
		float denom = (A1*B2) - (A2*B1);

		if (denom == 0) {
			return RayHit();
		}

		float ix = ((B2*C1) - (B1*C2)) / denom;
		float iy = ((A1*C2) - (A2*C1)) / denom;

		float rx1 = (ix - x1) / (x2 - x1);
		float ry1 = (iy - y1) / (y2 - y1);
		float rx2 = (ix - x3) / (x4 - x3);
		float ry2 = (iy - y3) / (y4 - y3);

		if (((rx1 >= 0 && rx1 <= 1) || (ry1 >= 0 && ry1 <= 1)) &&
			((rx2 >= 0 && rx2 <= 1) || (ry2 >= 0 && ry2 <= 1))) {
			return RayHit(glm::vec2(ix, iy), nullptr);
		}

		return RayHit(false);
	}

	RayHit RaytoAABB(Ray ray, AABB box) {
		glm::vec2 tl = box.m_min; //box topleft
		glm::vec2 lr = box.m_max; //box lowerright

		RayHit up		= RaytoRay(ray, Ray(tl.x, tl.y, lr.x, tl.y));
		RayHit down		= RaytoRay(ray, Ray(tl.x, lr.y, lr.x, lr.y));
		RayHit left		= RaytoRay(ray, Ray(tl.x, tl.y, tl.x, lr.y));
		RayHit right	= RaytoRay(ray, Ray(lr.x, tl.y, lr.x, lr.y));

		float closest = ray.distance();
		RayHit closestSide;
		if (up.hit && glm::length(up.position - ray.origin) <= closest) {
			closest = glm::length(up.position - ray.origin) <= closest;
			closestSide = up;
		}
		if (down.hit && glm::length(down.position - ray.origin) <= closest) {
			closest = glm::length(down.position - ray.origin) <= closest;
			closestSide = down;
		}
		if (left.hit && glm::length(left.position - ray.origin) <= closest) {
			closest = glm::length(left.position - ray.origin) <= closest;
			closestSide = left;
		}
		if (right.hit && glm::length(right.position - ray.origin) <= closest) {
			closest = glm::length(right.position - ray.origin) <= closest;
			closestSide = right;
		}

		return closestSide;
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
			if (m_objectA->GetGameObject()->Active() && m_objectB->GetGameObject()->Active()) {
				//check shape
				if (m_objectA->GetType() == COLLIDER_TYPE::BOX
					&& m_objectB->GetType() == COLLIDER_TYPE::BOX)
				{
					return (AABBtoAABB(this));
				}
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
			float meanFriction = (m_objectB->GetFriction() + m_objectA->GetFriction()) / 2.0f;

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
				glm::vec3 vel = m_objectA->m_rigidbody->GetVelocity();
				vel.x *= (1.0f - meanFriction);

				m_objectA->m_rigidbody->SetVelocity(vel);
				m_objectA->m_rigidbody->SetVelocity0Masked(-resultVec);
				//ENGINE_INFO("objA {}", m_objectA->GetGameObject()->GetID());
			}

			if (m_objBResFlag)
			{
				glm::vec3 vel = m_objectB->m_rigidbody->GetVelocity();
				vel.x *= (1.0f - meanFriction);

				m_objectB->m_rigidbody->SetVelocity(m_objectB->m_rigidbody->GetVelocity()*(1.0f - meanFriction));
				m_objectB->m_rigidbody->SetVelocity0Masked(resultVec);
				//ENGINE_INFO("objB {}", m_objectB->GetGameObject()->GetID());
			}
			//ENGINE_INFO("reslv\n");
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
