#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/GameObject.hpp"
#include "Physic/Collision.hpp"
#include "Physic/PhysicScene.hpp"

namespace Physic
{
	RayHits PhysicScene::RaycastAll(Ray ray, Layer targetLayer) {
		RayHits hits;
		//Check collision with layer to check
		for (auto collider : m_colliders[targetLayer])
		{
			//check collision
			if (collider != nullptr)
			{
				if (collider->GetGameObject()->Active()) {
					//check shape
					if (collider->GetType() == COLLIDER_TYPE::BOX)
					{
						std::shared_ptr<BoxCollider> box = dynamic_pointer_cast<BoxCollider>(collider);
						AABB aabb;
						box->ComputeAABB(aabb);

						RayHit Hit = RaytoAABB(ray, aabb);
						if (Hit.hit) {
							Hit.collider = collider;
							hits.push_back(Hit);
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

	RayHit PhysicScene::Raycast(Ray ray, Layer targetLayer) {
		RayHits hits = RaycastAll(ray, targetLayer);
		float closestDistance = ray.distance();
		RayHit closestHit;

		//find RayHit closest to origin
		for (RayHit hit : hits) {
			float dist = glm::length(hit.position - ray.origin);

			if (dist <= closestDistance) {
				closestDistance = dist;
				closestHit = hit;
			}
		}

		return closestHit;
	}
}