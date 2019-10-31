#include "Physic/PhysicScene.hpp"


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

		return LayerBit(static_cast<unsigned>(layer));
	}

	void PhysicScene::Update(float dt)
	{
		//Update Collision
		UpdateLayerCollision();

		//Check for duplicate pair
		CheckDuplicatePair();

		//Resolve Collision
		ResolveLayerCollision();
	}

	void PhysicScene::UpdateLayerCollision()
	{
		//Loop throuch every layer
		for (auto mainLayer : layerEnumArr)
		{
			//Set main layer
			LayerBit mainLayerBit = m_collisionLayer[mainLayer];

			if (mainLayerBit.count() == 0)
			{
				//Loop through main layer bit to get layer to check with
				for (auto layerToCheck : layerEnumArr)
				{
					//Get layerToCheck in LayerBit type
					LayerBit layerToCheckBit = GetLayerInBit(layerToCheck);

					//Check mainLayerBit with layerToCheckBit
					if ((mainLayerBit & layerToCheckBit) == layerToCheckBit)
					{
						CheckLayerCollision(mainLayer, layerToCheck);
					}
				}
			}
			
		}
	}

	void PhysicScene::ResolveLayerCollision()
	{
		if (m_collisionPairs.size() != 0)
		{
			for (auto colPair = m_collisionPairs.begin(); colPair != m_collisionPairs.end(); ++colPair)
			{
				//Resolve collision pair
				//TODO: Create result struct that will be pass into the collider
				//TODO: Generate collision result struct for the pair
			}
		}
	}

	void PhysicScene::CheckDuplicatePair()
	{
		for (auto it = m_collisionPairs.begin(); it != m_collisionPairs.end(); ++it)
		{
			CollisionPairs::iterator iterator = std::find(it+1, m_collisionPairs.end(), it);
			
			if (it == m_collisionPairs.end())
			{
				continue;
			}

			m_collisionPairs.erase(iterator);
		}
	}

	void PhysicScene::CheckLayerCollision(Layer mainLayer, Layer layerToCheck)
	{
		//Check collision of main collider with layer to check
		for (auto mainCol : m_colliders[mainLayer])
		{
			for (auto colToCheck : m_colliders[layerToCheck])
			{
				if (mainCol->GetType() == ColliderType::BOX)
				{
					if (colToCheck->GetType() == ColliderType::BOX)
					{
						//Call AABBtoAABB

					}
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
		m_collisionLayer[layer].set(static_cast<uint32_t>(layerToCollide));
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
		m_collisionLayer[layer].set(static_cast<uint32_t>(layerToCollide), false);
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
		if (m_layerString.find(layerName) == m_layerString.end())
		{
			ENGINE_ERROR("Undefined layer name: {}", layerName);
			return Layer::LAYER_INVALID;
		}
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

	PhysicScene::PhysicScene()
	{
		//To prevent wrong layer string
		m_layerString["INV"] = Layer::LAYER_INVALID;
	}
}


