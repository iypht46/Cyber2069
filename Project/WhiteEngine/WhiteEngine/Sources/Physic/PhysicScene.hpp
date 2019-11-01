//#pragma once
//
//#include <vector>
//#include <unordered_map>
//#include <bitset>
//#include <utility>
//#include <algorithm>
//#include <string>
//
//#include "Core/EC/Components/Collider.hpp"
//#include "Core/Logger.hpp"
//
//namespace Physic
//{
//	
//#define LAYER_BIT 8
//
//	enum class Layer : unsigned
//	{
//		LAYER_INVALID = 0x00,
//		PHYSIC_LAYER_1 = 0x01,
//		PHYSIC_LAYER_2 = 0x02,
//		PHYSIC_LAYER_3 = 0x03,
//		PHYSIC_LAYER_4 = 0x04,
//		PHYSIC_LAYER_5 = 0x05,
//		PHYSIC_LAYER_6 = 0x06,
//		PHYSIC_LAYER_7 = 0x07,
//		PHYSIC_LAYER_8 = 0x08
//	};
//
//	struct CollisionPair
//	{
//		std::pair<Collider*, Collider*> m_pair;
//
//		inline bool operator==(const CollisionPair& other) const
//		{
//			if (m_pair.first == other.m_pair.first)
//			{
//				if (m_pair.second == other.m_pair.second)
//				{
//					return true;
//				}
//				else
//				{
//					return false;
//				}
//			}
//			else
//			{
//				if (m_pair.first == other.m_pair.second)
//				{
//					if (m_pair.second == other.m_pair.first)
//					{
//						return true;
//					}
//					else
//					{
//						return false;
//					}
//				}
//			}
//
//		}
//	};
//
//	using LayerBit = std::bitset<LAYER_BIT>;
//	using Colliders = std::vector<Collider*>;
//	using ColliderMap = std::unordered_map<Layer, Colliders>;
//	using CollisionLayer = std::unordered_map<Layer, LayerBit>;
//	using CollisionPairs = std::vector<CollisionPair>;
//	using LayerStringMap = std::unordered_map<std::string, Layer>;
//
//	class PhysicScene
//	{
//	private:
//		ColliderMap m_colliders;
//		CollisionLayer m_collisionLayer;
//		CollisionPairs m_collisionPairs;
//		LayerStringMap m_layerString;
//	public:
//		//Main update loop
//		void Update(float);
//
//		//Update layer collision and generate pair
//		void UpdateLayerCollision();
//
//		//Resolve layer collision pair
//		void ResolveLayerCollision();
//
//		//Check for and remove duplicate collision pair
//		void CheckDuplicatePair();
//
//		//Check collision of a layer with another one
//		void CheckLayerCollision(Layer, Layer);
//
//		//
//		void CollisionCheck(Collider, Collider);
//
//		//Interface
//		//Add collider to layer
//		void Add(Collider*, Layer);
//		void Add(Collider*, std::string);
//		
//		//Remove collider from layer
//		void Remove(Collider*, Layer);
//		void Remove(Collider*, std::string);
//		
//		//Set Layer to Collide With
//		void SetLayerCollisions(Layer, Layer);
//		void SetLayerCollisions(std::string, std::string);
//
//		//Reset Layer to Collide With
//		void ResetLayerCollisions(Layer, Layer);
//		void ResetLayerCollisions(std::string, std::string);
//
//		//Get Layer to collide with
//		LayerBit GetLayerCollisions(Layer);
//		LayerBit GetLayerCollisions(std::string);
//
//		//Map string with layer enum
//		void SetLayerName(std::string, Layer);
//		//Get layer enum from layer string map
//		Layer GetLayerFromString(std::string);
//		//Get string from layer
//		std::string GetStringFromLayer(Layer);
//
//		PhysicScene();
//		~PhysicScene() {}
//	};
//}
//
//
//
