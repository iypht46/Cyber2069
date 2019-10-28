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
		switch (layer)
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
		}
	}

	void PhysicScene::Update(float dt)
	{
		//Update Collision
		UpdateLayerCollision();

		//Check for duplicate pair

		//Resolve Collision
		ResolveLayerCollision();
	}

	void PhysicScene::UpdateLayerCollision()
	{
		for (auto layer : layerEnumArr)
		{

		}
	}

	void PhysicScene::ResolveLayerCollision()
	{

	}

	void PhysicScene::Add(Collider* col, Layer layer)
	{
		m_colliders[layer].push_back(col);
	}

	void PhysicScene::Add(Collider* col, std::string layerName)
	{
		if (m_layerString.find(layerName) == m_layerString.end())
		{
			return;
		}
		
		m_colliders[m_layerString[layerName]].push_back(col);
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
		m_layerCollision[layer].set(static_cast<uint32_t>(layerToCollide));
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
		m_layerCollision[layer].set(static_cast<uint32_t>(layerToCollide), false);
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
		return m_layerCollision[layer];
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
			return m_layerCollision[m_layerString[layerName]];
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

	/*std::string PhysicScene::GetStringFromLayer(Layer layer)
	{
		
	}*/
}


