#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/GameObject.hpp"
#include "Physic/Collision.hpp"
#include "Physic/PhysicScene.hpp"

namespace Physic
{
	Colliders PhysicScene::RaycastAll(Ray ray, Layer targetLayer) {
		Colliders hits;
		//Check collision with layer to check
		for (auto* collider : m_colliders[targetLayer])
		{
			//check collision
			if (collider != nullptr)
			{
				if (collider->GetGameObject()->Active()) {
					//check shape
					if (collider->GetType() == COLLIDER_TYPE::BOX)
					{
						BoxCollider* box = dynamic_cast<BoxCollider*>(collider);
						AABB aabb;
						box->ComputeAABB(aabb);

						if (RaytoAABB(ray, aabb)) {
							hits.push_back(collider);
						}
					}
				}
			}
			else
			{
				ENGINE_ERROR("Object reference in Manifold is NULL");
			}
		}
		return hits;
	}

	Collider* PhysicScene::Raycast(Ray ray, Layer targetLayer) {
		Colliders hits = RaycastAll(ray, targetLayer);
		float closestDistance = ray.distance();
		Collider* closestHit = nullptr;

		//find collider closest to origin
		for (Collider* coll : hits) {
			float dist = glm::length(coll->m_transform->GetPosition() - glm::vec3(ray.origin, 0.0));
			if (dist < closestDistance) {
				closestDistance = dist;
				closestHit = coll;
			}
		}

		return closestHit;
	}
}