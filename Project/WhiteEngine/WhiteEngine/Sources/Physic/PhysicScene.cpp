#include "Physic/PhysicScene.hpp"
#include <iostream>

namespace Physic
{

	Layer layerEnumArr [] =
	{
		Layer::PHYSIC_LAYER_1,
		Layer::PHYSIC_LAYER_2,
		Layer::PHYSIC_LAYER_3,
		Layer::PHYSIC_LAYER_4,
		Layer::PHYSIC_LAYER_5,
		Layer::PHYSIC_LAYER_6,
		Layer::PHYSIC_LAYER_7,
		Layer::PHYSIC_LAYER_8,
	};

	LayerBit GetLayerInBit(Layer layer)
	{
		/*switch (layer)
		{
		case Physic::Layer::PHYSIC_LAYER_1:
			return LayerBit(1);
		case Physic::Layer::PHYSIC_LAYER_2:
			return LayerBit(2);
		case Physic::Layer::PHYSIC_LAYER_3:
			return LayerBit(3);
		case Physic::Layer::PHYSIC_LAYER_4:
			return LayerBit(4);
		case Physic::Layer::PHYSIC_LAYER_5:
			return LayerBit(5);
		case Physic::Layer::PHYSIC_LAYER_6:
			return LayerBit(6);
		case Physic::Layer::PHYSIC_LAYER_7:
			return LayerBit(7);
		case Physic::Layer::PHYSIC_LAYER_8:
			return LayerBit(8);
		default:
			return LayerBit(0);
		}*/

		return LayerBit(PhysicScene::LayerToNum(layer));
	}

	uint32_t PhysicScene::LayerToNum(Layer layer)
	{
		return (static_cast<uint32_t>(layer));
	}

	bool SortCollision(Manifold lhs, Manifold rhs)
	{
		if (lhs.m_objectA < rhs.m_objectA)
			return true;

		if (lhs.m_objectA == rhs.m_objectA)
			return lhs.m_objectB < rhs.m_objectB;

		return false;
	}

	void PhysicScene::Update(float dt)
	{
		//Apply Gravity
		ApplyGravity(dt);

		//Update Transform
		UpdateTransform(dt);

		//UpdateTransform(dt);
		//Update Collision
		UpdateLayerCollision();

		//Check for duplicate pair
		CheckDuplicatePair();

		

		//Resolve Collision
		ResolveLayerCollision(dt);

		//Update Rigid bodies transform
		UpdateTransform(dt);
	}

	void PhysicScene::UpdateLayerCollision()
	{
		//Clear Previous Collision
		m_possibleCollision.clear();

		//Loop throuch every layer
		for (auto mainLayer : m_collisionLayer)
		{
			//Set main layer
			LayerBit mainLayerBit = mainLayer.second;

			if (mainLayerBit.count() != 0)
			{
				//Loop through main layer bit to get layer to check with
				for (auto layerToCheck : layerEnumArr)
				{
					if (mainLayer.first == layerToCheck)
						continue;

					//Get layerToCheck in LayerBit type
					LayerBit layerToCheckBit = GetLayerInBit(layerToCheck);

					//Check mainLayerBit with layerToCheckBit
					if ((mainLayerBit & layerToCheckBit) == layerToCheckBit)
					{
						CheckLayerCollision(mainLayer.first, layerToCheck);
						//ENGINE_INFO("Main Layer: {}, LayerToCheck: {}", LayerToNum(mainLayer.first), LayerToNum(layerToCheck));
					}
				}
			}

		}
	}

	void PhysicScene::ResolveLayerCollision(float dt)
	{

		if (m_finalCollision.size() != 0)
		{
			for (auto colPair = m_finalCollision.begin(); colPair != m_finalCollision.end(); ++colPair)
			{
				//Resolve collision pair

				//ENGINE_INFO("Collision: {}", m_finalCollision.size());
				colPair->Resolve(dt);
			}

			m_finalCollision.clear();
		}
	}

	void PhysicScene::UpdateTransform(float dt)
	{
		for (auto body = m_bodies.begin(); body != m_bodies.end(); ++body)
		{
			(*body)->UpdateTransform(dt);
		}
	}
	
	void PhysicScene::ApplyGravity(float dt)
	{
		for (auto it = m_bodies.begin(); it != m_bodies.end(); ++it)
		{
			Rigidbody* body = (*it);

			body->AddForce(m_gravity * body->GetGravityScale() * body->GetMass() * GRAVITY_MUL);
		}
	}
	
	void PhysicScene::CheckDuplicatePair()
	{
		//ENGINE_INFO("Collision: {}", m_possibleCollision.size());

		std::sort(m_possibleCollision.begin(), m_possibleCollision.end(), SortCollision);

		int i = 0;
		while (i < m_possibleCollision.size())
		{
			Manifold* col = &(*(m_possibleCollision.begin() + i));

			m_finalCollision.push_back(*col);

			++i;

			while (i < m_possibleCollision.size())
			{
				Manifold* colDup = &(*(m_possibleCollision.begin() + i));

				if (col->m_objectA != colDup->m_objectB || col->m_objectB != colDup->m_objectA)
				{
					break;
				}

				++i;
			}

		}
	}

	void PhysicScene::CheckLayerCollision(Layer mainLayer, Layer layerToCheck)
	{
		//ENGINE_INFO("Check Collision");
		//ENGINE_INFO("Main: {}, LayerToCheck: {}", LayerToNum(mainLayer), LayerToNum(layerToCheck));
		//Check collision of main collider with layer to check
		for (auto mainCol : m_colliders[mainLayer])
		{

			for (auto colToCheck : m_colliders[layerToCheck])
			{
				//ENGINE_INFO("Object Collided");
				if (mainCol == colToCheck)
					continue;

				Manifold collision(mainCol, colToCheck);

				if (collision.CheckCollision())
				{
					//ENGINE_INFO("Object Collided");
					m_possibleCollision.push_back(collision);
				}
			}
		}
	}

	void PhysicScene::Add(Collider* col, Layer layer)
	{
		m_colliders[layer].push_back(col);
	}

	void PhysicScene::Add(Collider* col, std::string layerName)
	{
		Layer layer = GetLayerFromString(layerName);
		if (layer == Layer::LAYER_INVALID)
		{
			return;
		}

		Add(col, layer);
	}

	void PhysicScene::Add(Rigidbody* rigid)
	{
		m_bodies.push_back(rigid);
	}

	void PhysicScene::Remove(Collider* col, Layer layer)
	{
		Colliders::iterator it = std::find(m_colliders[layer].begin(), m_colliders[layer].end(), col);
		if (it == m_colliders[layer].end())
		{
			ENGINE_ERROR("Collider not found in layer: {}", layer);
			return;
		}

		m_colliders[layer].erase(it);
	}

	void PhysicScene::Remove(Collider* col, std::string layerName)
	{
		Layer layer = GetLayerFromString(layerName);
		if (layer == Layer::LAYER_INVALID)
		{
			return;
		}

		Remove(col, layer);
	}

	void PhysicScene::SetLayerCollisions(Layer layer, Layer layerToCollide)
	{
		uint32_t lay = LayerToNum(layerToCollide);
		m_collisionLayer[layer] |= static_cast<unsigned>(layerToCollide);
	}

	void PhysicScene::SetLayerCollisions(std::string layer, std::string layerToCollide)
	{
		Layer mainLayer = GetLayerFromString(layer);
		Layer layerToAdd = GetLayerFromString(layerToCollide);

		if (mainLayer == Layer::LAYER_INVALID
			|| layerToAdd == Layer::LAYER_INVALID)
		{
			ENGINE_ERROR("Failed setting layer collisions {} to {}", layerToCollide, layer);
			return;
		}

		SetLayerCollisions(mainLayer, layerToAdd);

	}

	void PhysicScene::ResetLayerCollisions(Layer layer, Layer layerToCollide)
	{
		m_collisionLayer[layer].set(LayerToNum(layerToCollide), false);
	}

	void PhysicScene::ResetLayerCollisions(std::string layer, std::string layerToCollide)
	{
		Layer mainLayer = GetLayerFromString(layer);
		Layer layerToAdd = GetLayerFromString(layerToCollide);

		if (mainLayer == Layer::LAYER_INVALID
			|| layerToAdd == Layer::LAYER_INVALID)
		{
			ENGINE_ERROR("Failed setting layer collisions {} to {}", layerToCollide, layer);
			return;
		}

		ResetLayerCollisions(mainLayer, layerToAdd);
	}

	LayerBit PhysicScene::GetLayerCollisions(Layer layer)
	{
		return m_collisionLayer[layer];
	}

	LayerBit PhysicScene::GetLayerCollisions(std::string layerName)
	{
		Layer layer = GetLayerFromString(layerName);

		if (layer == Layer::LAYER_INVALID)
		{
			return LayerBit(0);
		}
		else
		{
			return m_collisionLayer[m_layerString[layerName]];
		}
	}

	void PhysicScene::SetLayerName(std::string layerName, Layer layerToMap)
	{
		m_layerString[layerName] = layerToMap;
	}

	Layer PhysicScene::GetLayerFromString(std::string layerName)
	{
		auto layer = m_layerString.find(layerName);
		if (layer == m_layerString.end())
		{
			ENGINE_ERROR("Undefined layer name: {}", layerName);
			return Layer::LAYER_INVALID;
		}

		return layer->second;
	}

	std::string PhysicScene::GetStringFromLayer(Layer layer)
	{
		for (auto it = m_layerString.begin(); it != m_layerString.end(); ++it)
		{
			if (it->second == layer)
			{
				return it->first;
			}
		}

		std::string layerName = "INV";
		ENGINE_ERROR("{}, is not map to any named", layerName);
		return layerName;
	}

	void PhysicScene::SetGravity(glm::vec3 value)
	{
		m_gravity = value;
	}

	PhysicScene::PhysicScene()
	{
		m_gravity = glm::vec3(0.0f, -10.0f, 0.0f);
		//To prevent wrong layer string
		m_layerString["INV"] = Layer::LAYER_INVALID;
	}
}
